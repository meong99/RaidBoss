// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Character/Ability_ControlMovementDirection.h"
#include "AbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Management/RaidBossGameplayTags.h"

bool UAbility_ControlMovementDirection::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                           const FGameplayAbilityActorInfo* ActorInfo,
                                                           const FGameplayTagContainer* SourceTags,
                                                           const FGameplayTagContainer* TargetTags,
                                                           FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bIsValidOwner = OwnerCharacter ? true : false;

    return  bIsValidOwner &&
            Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UAbility_ControlMovementDirection::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AlignThreshold = FMath::Clamp(AlignThreshold, 0, 1);

    UAbilitySystemComponent*    AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    FRotator                    NewRotator = GetRotationByMovementInput();

    if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(RaidBossGameplayTags::Get().Attack))
    {
        RotateToForward();
    }
    else if (NewRotator.IsZero() == false)
    {
        RotateByMovementInput(NewRotator);
    }
    else if (NewRotator.IsZero())
    {
        RotateToForwardWithThreshold();
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAbility_ControlMovementDirection::RotateToForward()
{
    FRotator NewRotator = FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0};

    if (NewRotator.IsZero() == false)
    {
        NewRotator = FMath::RInterpTo(OwnerCharacter->GetActorRotation(), NewRotator, GetWorld()->DeltaTimeSeconds,InterpSpeedAtAttack);
        
        OwnerCharacter->SetActorRotation(NewRotator);
    }
}

void UAbility_ControlMovementDirection::RotateByMovementInput(FRotator NewRotator)
{
    bIsDuringAlign  = false;
    NewRotator      = FMath::RInterpTo(OwnerCharacter->GetActorRotation(), NewRotator, GetWorld()->DeltaTimeSeconds, InterpSpeed);
    
    OwnerCharacter->SetActorRotation(NewRotator);
}

void UAbility_ControlMovementDirection::RotateToForwardWithThreshold()
{
    FRotator NewRotator = GetAlignedRotationByForwardVector();

    if (NewRotator.IsZero() == false)
    {
        NewRotator = FMath::RInterpConstantTo(OwnerCharacter->GetActorRotation(), NewRotator,
                                              GetWorld()->DeltaTimeSeconds, AlignSpeed);
        
        OwnerCharacter->SetActorRotation(NewRotator);
    }
}

FRotator UAbility_ControlMovementDirection::GetRotationByMovementInput() const
{
    float OutMoveForward, OutMoveRight;

    GetMovementInput(OutMoveForward, OutMoveRight);

    FRotator Result = {0, 0, 0};

    if (bool bIsOnlyForward = OutMoveForward && FMath::IsNearlyEqual(OutMoveRight, 0))
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0};
    }
    else if (bool bIsOnlyRight = OutMoveRight > 0 && FMath::IsNearlyEqual(OutMoveForward, 0))
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw + 90, 0};
    }
    else if (bool bIsOnlyLeft = OutMoveRight < 0 && FMath::IsNearlyEqual(OutMoveForward, 0))
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw - 90, 0};
    }
    else if (bool bIsForwardRight = OutMoveRight > 0 && OutMoveForward > 0)
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw + 45, 0};
    }
    else if (bool bIsForwardLeft = OutMoveRight < 0 && OutMoveForward > 0)
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw - 45, 0};
    }
    else if (bool bIsBackwardRight = OutMoveRight > 0 && OutMoveForward < 0)
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw - 45, 0};
    }
    else if (bool bIsBackwardLeft = OutMoveRight < 0 && OutMoveForward < 0)
    {
        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw + 45, 0};
    }

    return Result;
}

FRotator UAbility_ControlMovementDirection::GetAlignedRotationByForwardVector()
{
    FVector     ControlForward = UKismetMathLibrary::GetForwardVector(OwnerCharacter->GetControlRotation());
    float       ForwardByForwardCosine = UKismetMathLibrary::Vector_CosineAngle2D(ControlForward, OwnerCharacter->GetActorForwardVector());
    float       ForwardByRightCosine = UKismetMathLibrary::Vector_CosineAngle2D(ControlForward, OwnerCharacter->GetActorRightVector());
    FRotator    Result = {0, 0, 0};

    if (ForwardByForwardCosine < AlignThreshold || bIsDuringAlign)
    {
        bIsDuringAlign = true;

        Result = FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0};
    }

    if (ForwardByRightCosine > 0)
    {
        OwnerCharacter->SetIsTurnLeft(false);
    }
    else
    {
        OwnerCharacter->SetIsTurnLeft(true);
    }

    if (FMath::IsNearlyEqual(ForwardByForwardCosine, 1))
    {
        bIsDuringAlign = false;
    }

    OwnerCharacter->SetIsDuringAlign(bIsDuringAlign);

    return Result;
}

void UAbility_ControlMovementDirection::GetMovementInput(float& OutMoveForward, float& OutMoveRight) const
{
    OutMoveForward  = OwnerCharacter->GetInputMoveForward();
    OutMoveRight    = OwnerCharacter->GetInputMoveRight();
}
