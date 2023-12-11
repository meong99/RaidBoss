// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Sevarog/Ability_Sevarog_RMB.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAbility_Sevarog_RMB::UAbility_Sevarog_RMB()
{
}

void UAbility_Sevarog_RMB::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    OriginMaxWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;

    auto    PlayMontageAnsWait = CreatePlayMontageAndWaitTask();

    if (PlayMontageAnsWait)
    {
        ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 3;
        OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);

        PlayMontageAnsWait->OnCancelled.AddDynamic(this, &UAbility_Sevarog_RMB::EndAbilityCallback);
        PlayMontageAnsWait->OnCompleted.AddDynamic(this, &UAbility_Sevarog_RMB::EndAbilityCallback);
        PlayMontageAnsWait->OnInterrupted.AddDynamic(this, &UAbility_Sevarog_RMB::EndAbilityCallback);
        PlayMontageAnsWait->OnBlendOut.AddDynamic(this, &UAbility_Sevarog_RMB::EndAbilityCallback);

        PlayMontageAnsWait->ReadyForActivation();
    }
    else
    {
        EndAbilityCallback();
    }
}

void UAbility_Sevarog_RMB::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
    if (OwnerCharacter && OriginMaxWalkSpeed)
    {
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginMaxWalkSpeed;
    }
    if (OwnerCharacter)
    {
        OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Sevarog_RMB::EndAbilityCallback()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
