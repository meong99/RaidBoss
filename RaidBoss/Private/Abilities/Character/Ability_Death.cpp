// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Character/Ability_Death.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/RaidBossEnemyControllerBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"

void UAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
    OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
    OwnerCharacter->SetCurrentPlayerState(ECharacterState::None);
    OwnerCharacter->SetIsMovementBlocked(true);

    UAbilityTask_PlayMontageAndWait* PlayMontageAndWait =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(GetName()),
                                                                       OwnerCharacter->GetCharacterAnimations().DeathMontage);

    if (PlayMontageAndWait)
    {
        PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Death::EndAbilityCallBak);
        PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Death::EndAbilityCallBak);
        PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Death::EndAbilityCallBak);
        PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Death::EndAbilityCallBak);

        PlayMontageAndWait->ReadyForActivation();
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAbility_Death::EndAbilityCallBak()
{
    if (OwnerCharacter)
    {
        OwnerCharacter->UnEquipWeapon();
        OwnerCharacter->SetCurrentPlayerState(ECharacterState::Dead);
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
