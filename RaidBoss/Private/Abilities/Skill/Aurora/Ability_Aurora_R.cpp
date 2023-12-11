// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Aurora/Ability_Aurora_R.h"
#include "Skill/Indicator/SkillIndicator.h"
#include "Abilities/Task/AT_FloatingActor.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

UAbility_Aurora_R::UAbility_Aurora_R()
{
    SkillDamageRate = 5.f;
}

void UAbility_Aurora_R::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (bIsFirstTrigger)
    {
        SetIndicator();
        
        bIsFirstTrigger = false;
    }
    else
    {
        bIsFirstTrigger = true;
        
        UAbilityTask_PlayMontageAndWait*    PlayMontageAndWait = CreatePlayMontageAndWaitTask();
        UAbilityTask_WaitGameplayEvent*     WaitGameplayEvent =
            CreateWaitGameplayEventTask(RaidBossGameplayTags::Get().Animation_Notify_AttackPoint);
        UAT_FloatingActor*                  FloatingActorTask =
            UAT_FloatingActor::CreateFloatingActorTask(this, FloatingHeight, RisingSpeed, FloatingTime, FallingSpeed);

        if (PlayMontageAndWait && WaitGameplayEvent && FloatingActorTask)
        {
            ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

            OriginFlySpeed = OwnerCharacter->GetCharacterMovement()->MaxFlySpeed;
            
            OwnerCharacter->SetCanActivateNormalAttack(false);
            OwnerCharacter->GetCharacterMovement()->MaxFlySpeed = FloatingMoveSpeed;
            PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Aurora_R::EndAbilityCallback);
            PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Aurora_R::EndAbilityCallback);
            PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Aurora_R::EndAbilityCallback);
            PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Aurora_R::EndAbilityCallback);

            WaitGameplayEvent->EventReceived.AddDynamic(this, &UAbility_Aurora_R::AttackPointCallback);

            PlayMontageAndWait->ReadyForActivation();
            WaitGameplayEvent->ReadyForActivation();
            FloatingActorTask->ReadyForActivation();
        }
        else
        {
            EndAbilityCallback();
        }
    }
}

void UAbility_Aurora_R::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateCancelAbility)
{
    bIsFirstTrigger = true;
    
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAbility_Aurora_R::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                   bool bWasCancelled)
{
    OwnerCharacter->SetCanActivateNormalAttack(true);
    
    if (OriginFlySpeed > 0)
    {
        OwnerCharacter->GetCharacterMovement()->MaxFlySpeed = OriginFlySpeed;
    }
    
    OriginFlySpeed = 0;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Aurora_R::SetIndicator()
{
    for (auto Indicator : TestIndicators)
    {
        UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent =
            UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(this, Indicator, OwnerCharacter, 0,
                                                                     FVector{1, SkillRadius, SkillRadius}, bAttachLocationToParent,
                                                                     bFollowRotationToParent);

        if (SpawnActorAndFollowParent == nullptr)
        {
            continue;
        }
        
        SpawnActorAndFollowParent->ReadyForActivation();

        if (auto SpawnedIndicator = Cast<ASkillIndicator>(SpawnActorAndFollowParent->GetSpawnedActor()))
        {
            SpawnedIndicator->SetIndicatorColor(FColor::Blue);
        }
    }
}

void UAbility_Aurora_R::AttackPointCallback(FGameplayEventData EventData)
{
    TArray<TSubclassOf<AActor>> FilterClasses;

    if (Cast<AMonsterBase>(OwnerCharacter))
    {
        FilterClasses.Add(ARaidBossPlayerBase::StaticClass());
    }
    else
    {
        FilterClasses.Add(AMonsterBase::StaticClass());
    }

    TArray<AActor*> TargetActors = URaidBossLibrary::GetActorsInAngle(this, OwnerCharacter->GetActorLocation(),
                                                                      OwnerCharacter->GetActorForwardVector(),
                                                                      SkillRadius, FilterClasses, {}, 360);

    for (const auto& Target : TargetActors)
    {
        FGameplayEffectSpecHandle           EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
        FGameplayAbilityTargetDataHandle    TargetDataHandle = CreateAbilityTargetDataFromActor(Target);
        
        EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Damage_SetByCaller, SkillDamageRate);
        
        TArray<FActiveGameplayEffectHandle> AppliedEffectsHandle =
            ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
                                            EffectSpecHandle, TargetDataHandle);
    }
}

void UAbility_Aurora_R::EndAbilityCallback()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
