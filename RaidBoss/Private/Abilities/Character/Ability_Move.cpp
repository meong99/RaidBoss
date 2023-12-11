// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Character/Ability_Move.h"
#include "Character/RaidBossCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

bool UAbility_Move::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayTagContainer* SourceTags,
                                       const FGameplayTagContainer* TargetTags,
                                       FGameplayTagContainer* OptionalRelevantTags) const
{
    bool    bIsValidOwner = OwnerCharacter != nullptr;
    bool    bCanMove = false;
    bool    bIsNotDead = false;

    if (bIsValidOwner)
    {
        bCanMove    = OwnerCharacter->IsMovementBlocked() == false;
        bIsNotDead  = OwnerCharacter->GetCurrentCharacterState() == ECharacterState::Alive;
    }

    bool    bCanActivateAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags,
                                                         OptionalRelevantTags);

    return bIsValidOwner && bCanMove && bIsNotDead && bCanActivateAbility;
}

void UAbility_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (OwnerCharacter)
    {
        FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, OwnerCharacter->GetControlRotation().Yaw, 0.0f));
        FVector Right = UKismetMathLibrary::GetRightVector(FRotator(0.0f, OwnerCharacter->GetControlRotation().Yaw, 0.0f));

        OwnerCharacter->AddMovementInput(Forward, OwnerCharacter->GetInputMoveForward());
        OwnerCharacter->AddMovementInput(Right, OwnerCharacter->GetInputMoveRight());
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
