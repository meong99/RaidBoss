// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Aurora/Ability_Aurora_RMB.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

void UAbility_Aurora_RMB::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerCharacter->SetIsMovementBlocked(true);
	OwnerCharacter->SetCanActivateNormalAttack(false);
	OwnerCharacter->GetCharacterMovement()->Velocity = {0, 0, 0};
	OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	
	FVector LaunchDirection;
	FName	StartSection;

	GetDirectionAndMontageSection(LaunchDirection, StartSection);
	
	LaunchCharacter(LaunchDirection);

	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	
	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWait = CreatePlayMontageAndWaitTask(0, 1, StartSection);
	UAbilityTask_WaitGameplayEvent*		WaitGameplayEvent = CreateWaitGameplayEventTask(
		RaidBossGameplayTags::Get().Animation_Notify_ReleaseMovement);
	
	if (PlayMontageAndWait && WaitGameplayEvent)
	{
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Aurora_RMB::EndAbilityCallback);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Aurora_RMB::EndAbilityCallback);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Aurora_RMB::EndAbilityCallback);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Aurora_RMB::EndAbilityCallback);

		WaitGameplayEvent->EventReceived.AddDynamic(this, &UAbility_Aurora_RMB::ReleaseMovementCallback);
		
		PlayMontageAndWait->ReadyForActivation();
		WaitGameplayEvent->ReadyForActivation();
	}
	else
	{
		EndAbilityCallback();
	}
}

void UAbility_Aurora_RMB::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	OwnerCharacter->SetIsMovementBlocked(false);
	OwnerCharacter->SetCanActivateNormalAttack(true);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Aurora_RMB::EndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_Aurora_RMB::ReleaseMovementCallback(FGameplayEventData Payload)
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	}
}

void UAbility_Aurora_RMB::GetDirectionAndMontageSection(FVector& OutLaunchDirection, FName& OutStartSection)
{
	float		InputMoveForward = OwnerCharacter->GetInputMoveForward();
	float		InputMoveRight = OwnerCharacter->GetInputMoveRight();
	FVector		ControlFwdVector = UKismetMathLibrary::GetForwardVector(FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0});
	FVector		ControlRightVector = UKismetMathLibrary::GetRightVector(FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0});

	if (bool bIsOnlyForward = InputMoveForward >= 0 && FMath::IsNearlyZero(InputMoveRight))
	{
		OutStartSection = "Fwd";
		OutLaunchDirection = ControlFwdVector;
	}
	else if (bool bIsOnlyBwd = InputMoveForward < 0 && FMath::IsNearlyZero(InputMoveRight))
	{
		OutStartSection = "Bwd";
		OutLaunchDirection = ControlFwdVector * -1;
	}
	else if (bool bIsOnlyRight = InputMoveRight > 0 && FMath::IsNearlyZero(InputMoveForward))
	{
		OutStartSection = "Fwd";
		OutLaunchDirection = ControlRightVector;
	}
	else if (bool bIsOnlyLeft = InputMoveRight < 0 && FMath::IsNearlyZero(InputMoveForward))
	{
		OutStartSection = "Fwd";
		OutLaunchDirection = ControlRightVector * -1;
	}
	else if (bool bIsFwdRight = InputMoveForward > 0 && InputMoveRight > 0)
	{
		OutStartSection = "Fwd";
		OutLaunchDirection = (ControlFwdVector + ControlRightVector).GetSafeNormal();
	}
	else if (bool bIsFwdLeft = InputMoveForward > 0 && InputMoveRight < 0)
	{
		OutStartSection = "Fwd";
		OutLaunchDirection = (ControlFwdVector + ControlRightVector * -1).GetSafeNormal();
	}
	else if (bool bIsBwdRight = InputMoveForward < 0 && InputMoveRight > 0)
	{
		OutStartSection = "Bwd";
		OutLaunchDirection = (ControlFwdVector * -1 + ControlRightVector).GetSafeNormal();
	}
	else if (bool bIsBwdLeft = InputMoveForward < 0 && InputMoveRight < 0)
	{
		OutStartSection = "Bwd";
		OutLaunchDirection = (ControlFwdVector * -1 + ControlRightVector * -1).GetSafeNormal();
	}
}

void UAbility_Aurora_RMB::LaunchCharacter(FVector LaunchDirection)
{
	FVector OwnerWorldLocation = OwnerCharacter->GetActorLocation();
	FVector LaunchVelocity;

	URaidBossLibrary::GetVelocityToLaunchActor(this, LaunchVelocity, OwnerWorldLocation,
		OwnerWorldLocation + LaunchDirection * LaunchPower, LaunchArc);

	if (bDrawDebugLine)
	{
		FPredictProjectilePathParams Params;

		Params.StartLocation = OwnerWorldLocation;
		Params.LaunchVelocity = LaunchVelocity;
		Params.ProjectileRadius = 5.0f;
		Params.DrawDebugTime = 3.0f;
		Params.DrawDebugType = EDrawDebugTrace::ForDuration;
		
		FPredictProjectilePathResult Result;
		
		UGameplayStatics::PredictProjectilePath(this, Params, Result);
	}

	OwnerCharacter->LaunchCharacter(LaunchVelocity, false, false);
}
