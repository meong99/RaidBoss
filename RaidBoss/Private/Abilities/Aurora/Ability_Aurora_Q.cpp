// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Aurora/Ability_Aurora_Q.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Task/AT_DashCharacter.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Equipment/Weapon/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/DataPassingObject.h"

UAbility_Aurora_Q::UAbility_Aurora_Q()
{
}

bool UAbility_Aurora_Q::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool	bIsValidOwner = OwnerCharacter ? true : false;
	bool	bIsValidWeaponData = false;

	if (CurrentWeapon)
	{
		bIsValidWeaponData = true;
	}
	
	return	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) &&
			bIsValidOwner &&
			bIsValidWeaponData;
}

void UAbility_Aurora_Q::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	CurrentWeapon = Cast<AWeapon>(Spec.SourceObject);
}

void UAbility_Aurora_Q::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWait = nullptr;
	
	if (Montages.IsEmpty() == false)
	{
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, FName(GetName()), *Montages.begin(), true);
	}

	OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	OwnerCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UAbility_Aurora_Q::NotifyBeginOverlappedCallBack);
	
	DashCharacterTask = UAT_DashCharacter::CreateDashCharacterTask(
		this, UKismetMathLibrary::GetForwardVector(FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0}), DashSpeed, DashDistance);

	UAbilityTask_WaitGameplayEvent* WaitAttackPointEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, OwnerCharacter);

	if (PlayMontageAndWait && DashCharacterTask && WaitAttackPointEvent)
	{
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Aurora_Q::NotifyMontageEndedCallBack);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Aurora_Q::NotifyMontageEndedCallBack);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Aurora_Q::NotifyMontageEndedCallBack);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Aurora_Q::NotifyMontageEndedCallBack);

		WaitAttackPointEvent->EventReceived.AddDynamic(this, &UAbility_Aurora_Q::NotifyAttackPointEventCallBack);

		PlayMontageAndWait->ReadyForActivation();
		WaitAttackPointEvent->ReadyForActivation();
	}
}

void UAbility_Aurora_Q::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bIsFirstActivation = true;
	OwnerCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UAbility_Aurora_Q::NotifyBeginOverlappedCallBack);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	TargetActors.Reset();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Aurora_Q::NotifyMontageEndedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAbility_Aurora_Q::NotifyAttackPointEventCallBack(FGameplayEventData Payload)
{
	if (bIsFirstActivation)
	{
		DashCharacterTask->ReadyForActivation();
		bIsFirstActivation = false;
	}
	else
	{
		DashCharacterTask->ExternalCancel();
	}
}

void UAbility_Aurora_Q::NotifyBeginOverlappedCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TargetActors.Contains(SweepResult.GetActor()))
	{
		return;
	}
	
	FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SweepResult.GetActor());
	
	TArray<FActiveGameplayEffectHandle> EffectHandles =
		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, EffectClass, 1);

	FGameplayEventData EventData;

	EventData.Instigator = OwnerCharacter;
	EventData.EventMagnitude = KnockBackPower;
	EventData.OptionalObject = GetKnockBackData(SweepResult.GetActor());
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		SweepResult.GetActor(), RaidBossGameplayTags::Get().StatusEffect_KnockBack, EventData);
}

UDataPassingObject* UAbility_Aurora_Q::GetKnockBackData(AActor* TargetActor)
{
	if (TargetActor == nullptr)
	{
		return nullptr;
	}

	TargetActors.Add(TargetActor);
	
	UDataPassingObject*	PassingObject = NewObject<UDataPassingObject>();
	
	PassingObject->FloatValue.Add("KnockBackPower", KnockBackPower);

	FVector Direction;
	FVector	ToTargetDirection = (TargetActor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();

	if (OwnerCharacter->GetActorRightVector().Dot(ToTargetDirection) > 0)
	{
		Direction = OwnerCharacter->GetActorRightVector();
	}
	else
	{
		Direction = OwnerCharacter->GetActorRightVector() * -1;
	}
	
	PassingObject->VectorValue.Add("Direction", Direction);

	return PassingObject;
}
