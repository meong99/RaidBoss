// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/Ability_RangeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Equipment/Weapon/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Management/RaidBossGameplayTags.h"

UAbility_RangeAttack::UAbility_RangeAttack()
{
}

bool UAbility_RangeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) &&
		   bIsValidOwner &&
		   bIsValidWeaponData;
}

void UAbility_RangeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	CurrentWeapon = Cast<AWeapon>(Spec.SourceObject);
}

void UAbility_RangeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);

	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, FName(GetName()), *CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims.begin());
	
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, OwnerCharacter, true);

	if (PlayMontageAndWait && WaitGameplayEvent)
	{
		PlayMontageAndWait->OnCompleted.AddDynamic(this,	&UAbility_RangeAttack::EndAbilityCallBack);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this,		&UAbility_RangeAttack::EndAbilityCallBack);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this,	&UAbility_RangeAttack::EndAbilityCallBack);
		PlayMontageAndWait->OnCancelled.AddDynamic(this,	&UAbility_RangeAttack::EndAbilityCallBack);

		PlayMontageAndWait->ReadyForActivation();

		WaitGameplayEvent->EventReceived.AddDynamic(this,	&UAbility_RangeAttack::EventReceived);

		WaitGameplayEvent->ReadyForActivation();
	}
	else
	{
		EndAbilityCallBack();
	}
}

void UAbility_RangeAttack::EndAbilityCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_RangeAttack::EventReceived(FGameplayEventData Payload)
{
	FVector	StartPoint;
	FVector	EndPoint;

	SetTracePointsFromCamera(StartPoint, EndPoint);

	TArray<FHitResult> HitResults;

	StartTracingFromCamera(StartPoint, EndPoint, HitResults);
	SetTracePointsFromWeapon(StartPoint, EndPoint, HitResults);

	AActor*	TargetActor = nullptr;
	
	StartTracingFromWeapon(StartPoint, EndPoint, &TargetActor);
	if (TargetActor)
	{
		ApplyEffectToTarget(TargetActor);
	}
}

void UAbility_RangeAttack::SetTracePointsFromCamera(FVector& OutStartPoint, FVector& OutEndPoint)
{
	ARaidBossPlayerBase*	PlayerBase = Cast<ARaidBossPlayerBase>(OwnerCharacter);
	
	if (PlayerBase && CurrentWeapon)
	{
		OutStartPoint = PlayerBase->GetFollowCamera()->GetComponentLocation();

		const FVector	TraceVectorWithRange =
			UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation()) *
			(CurrentWeapon->GetWeaponData().AttackRange * 2);
		
		OutEndPoint = OutStartPoint + TraceVectorWithRange;
	}
}

void UAbility_RangeAttack::SetTracePointsFromWeapon(FVector& OutStartPoint, FVector& OutEndPoint, const TArray<FHitResult>& HitResultRef)
{
	OutStartPoint = CurrentWeapon ? CurrentWeapon->GetActorLocation() : OutStartPoint;
	
	for (auto Result : HitResultRef)
	{
		float	RemainingRange = CurrentWeapon->GetWeaponData().AttackRange - (Result.Location - OutStartPoint).Length();
		FVector NewEndPoint = Result.Location + (Result.Location - OutStartPoint).GetSafeNormal() * RemainingRange;
		
		ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(Result.GetActor());

		if (CharacterBase)
		{
			bool bIsDead = CharacterBase->GetCurrentPlayerState() == EPlayerState::Dead;
			bool bIsNone = CharacterBase->GetCurrentPlayerState() == EPlayerState::None;

			if (bIsDead == false && bIsNone == false)
			{
				OutEndPoint = NewEndPoint;
				break;
			}
		}
		else
		{
			OutEndPoint = NewEndPoint;
			break;
		}
	}
}

bool UAbility_RangeAttack::StartTracingFromCamera(const FVector& StartPoint, const FVector& EndPoint,
                                                  TArray<FHitResult>& OutHitResults)
{
	float	DrawDebugDuration = 0;
	TArray<AActor*> IgnoreActors = { OwnerCharacter };
	EDrawDebugTrace::Type drawDebugTrace = 0.f < DrawDebugDuration ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bWasHit = UKismetSystemLibrary::LineTraceMultiForObjects(
					OwnerCharacter->GetWorld(), StartPoint, EndPoint,
					{ UEngineTypes::ConvertToObjectType(ECC_Pawn), UEngineTypes::ConvertToObjectType(ECC_WorldStatic) },
					false, IgnoreActors, drawDebugTrace, OutHitResults, true,
					FLinearColor::Red, FLinearColor::Green, DrawDebugDuration);

	return bWasHit;
}

bool UAbility_RangeAttack::StartTracingFromWeapon(const FVector& StartPoint, const FVector& EndPoint,
	AActor** OutTargetActor)
{
	float	DrawDebugDuration = 0;
	TArray<AActor*> IgnoreActors = { OwnerCharacter };
	EDrawDebugTrace::Type drawDebugTrace = 0.f < DrawDebugDuration ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult> TempHitResults;

	bool bWasHit = UKismetSystemLibrary::LineTraceMultiForObjects(
					OwnerCharacter->GetWorld(), StartPoint, EndPoint,
					{ UEngineTypes::ConvertToObjectType(ECC_Pawn), UEngineTypes::ConvertToObjectType(ECC_WorldStatic) },
					false, IgnoreActors, drawDebugTrace, TempHitResults, true,
					FLinearColor::Red, FLinearColor::Green, DrawDebugDuration);
	
	for (FHitResult& TempHitResult : TempHitResults)
	{
		if (ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(TempHitResult.GetActor()))
		{
			if (CharacterBase->GetCurrentPlayerState() != EPlayerState::Dead &&
				CharacterBase->GetCurrentPlayerState() != EPlayerState::None)
			{
				*OutTargetActor = TempHitResult.GetActor();

				break;
			}
		}
	}
	
	return bWasHit;
}

void UAbility_RangeAttack::ApplyEffectToTarget(AActor* TargetActor)
{
	FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor);
		
	TArray<FActiveGameplayEffectHandle> EffectHandles =
		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, EffectClass, 1);
}
