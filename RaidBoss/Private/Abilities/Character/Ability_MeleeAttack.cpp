// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Character/Ability_MeleeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Equipment/Weapon/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Management/RaidBossGameplayTags.h"

bool UAbility_MeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                              const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool	bIsValidOwner = OwnerCharacter ? true : false;
	bool	bIsValidWeaponData = false;

	if (CurrentWeapon)
	{
		bIsValidWeaponData =
			CurrentWeapon->GetWeaponData().AttackRange > 0 &&
			CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims.Num() > 0;
	}
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return	bCanActivate &&
			bIsValidOwner &&
			bIsValidWeaponData &&
			bCanActivateNextAttack;
}

void UAbility_MeleeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	CurrentWeapon = Cast<AWeapon>(Spec.SourceObject);
}

void UAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);

	if (CurrentCombo == MaximumCombo)
	{
		CurrentCombo = ComboInitValue;
	}

	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWait = nullptr;
	
	if (CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims.IsValidIndex(CurrentCombo))
	{
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, FName(GetName()), CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims[CurrentCombo], true);
	}
	
	UAbilityTask_WaitGameplayEvent* WaitCanActivateNextAttackEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_CanActivateNextAttack, OwnerCharacter, true);
			
	UAbilityTask_WaitGameplayEvent* WaitAttackPointEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, OwnerCharacter, true);
	
	UAbilityTask_WaitGameplayEvent* WaitResetComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_ComboReset, OwnerCharacter, true);
	
	CurrentCombo++;
	bCanActivateNextAttack = false;
	
	if (PlayMontageAndWait && WaitAttackPointEvent && WaitResetComboEvent && WaitCanActivateNextAttackEvent)
	{
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_MeleeAttack::NotifyMontageCanceledCallBack);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_MeleeAttack::NotifyMontageCanceledCallBack);
		WaitAttackPointEvent->EventReceived.AddDynamic(this,&UAbility_MeleeAttack::NotifyAttackPointComesCallBack);
		WaitResetComboEvent->EventReceived.AddDynamic(this,	&UAbility_MeleeAttack::NotifyComboResetCallBack);
		WaitCanActivateNextAttackEvent->EventReceived.AddDynamic(this,	&UAbility_MeleeAttack::NotifyCanActivateNextAttackCallBack);
		
		WaitAttackPointEvent->ReadyForActivation();
		WaitResetComboEvent->ReadyForActivation();
		WaitCanActivateNextAttackEvent->ReadyForActivation();
		PlayMontageAndWait->ReadyForActivation();
	}
	else
	{
		NotifyMontageCanceledCallBack();
	}
}

void UAbility_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	HitActors.Reset();
	bCanActivateNextAttack = true;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_MeleeAttack::NotifyMontageCanceledCallBack()
{
	HitActors.Reset();
	bCanActivateNextAttack = true;
	CurrentCombo = ComboInitValue;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_MeleeAttack::NotifyAttackPointComesCallBack(FGameplayEventData Payload)
{
	FVector	StartPoint;
	FVector	EndPoint;

	SetTracePoints(StartPoint, EndPoint);

	TArray<FHitResult> HitResults;

	if (StartTracing(StartPoint, EndPoint, HitResults))
	{
		ApplyEffectToTarget();
	}
}

void UAbility_MeleeAttack::NotifyComboResetCallBack(FGameplayEventData Payload)
{
	CurrentCombo = ComboInitValue;
	NotifyMontageCanceledCallBack();
}

void UAbility_MeleeAttack::NotifyCanActivateNextAttackCallBack(FGameplayEventData Payload)
{
	bCanActivateNextAttack = true;
}

void UAbility_MeleeAttack::SetTracePoints(FVector& OutStartPoint, FVector& OutEndPoint)
{
	if (OwnerCharacter && CurrentWeapon)
	{
		OutStartPoint = CurrentWeapon->GetActorLocation();

		const FVector	TraceVectorWithRange =
			UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation()) *
			CurrentWeapon->GetWeaponData().AttackRange;
		
		OutEndPoint = OutStartPoint + TraceVectorWithRange;
	}
}

bool UAbility_MeleeAttack::StartTracing(const FVector& StartPoint, const FVector& EndPoint, TArray<FHitResult>& OutHitResults)
{
	float	TraceRadius = 100;
	float	DrawDebugDuration = 0;
	TArray<AActor*> IgnoreActors = { OwnerCharacter };
	EDrawDebugTrace::Type drawDebugTrace = 0.f < DrawDebugDuration ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult> TempHitResults;

	bool bWasHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
					OwnerCharacter->GetWorld(), StartPoint, EndPoint,
					TraceRadius, { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, false, IgnoreActors,
					drawDebugTrace, TempHitResults, true, FLinearColor::Red, FLinearColor::Green, DrawDebugDuration);

	for (FHitResult& TempHitResult : TempHitResults)
	{
		if (HitActors.Contains(TempHitResult.GetActor()) == false &&
			TempHitResult.GetActor()->Tags != OwnerCharacter->Tags)
		{
			HitActors.AddUnique(TempHitResult.GetActor());
			OutHitResults.Add(TempHitResult);
		}
	}
	
	return bWasHit;
}

void UAbility_MeleeAttack::ApplyEffectToTarget()
{
	for (const auto& TargetActor : HitActors)
	{
		FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor);
		
		TArray<FActiveGameplayEffectHandle> EffectHandles =
			ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, EffectClass, 1);
	}
}
