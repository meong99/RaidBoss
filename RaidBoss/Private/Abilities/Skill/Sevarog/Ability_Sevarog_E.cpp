// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Sevarog/Ability_Sevarog_E.h"
#include "Skill/Indicator/CircleIndicator.h"
#include "Abilities/Task/AT_SpawnActorAndFollowCamera.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

UAbility_Sevarog_E::UAbility_Sevarog_E()
{
    SkillDamageRate = 3.f;
}

void UAbility_Sevarog_E::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (bIsFirstTrigger && Cast<AMonsterBase>(OwnerCharacter) == nullptr)
    {
        SetIndicator();
        
        bIsFirstTrigger = false;
    }
    else
    {
        bIsFirstTrigger = true;
        
        auto PlayMontageAndWait = CreatePlayMontageAndWaitTask();
        auto WaitGameplayEvent = CreateWaitGameplayEventTask(RaidBossGameplayTags::Get().Animation_Notify_AttackPoint,
                                                             false);

        if (PlayMontageAndWait && WaitGameplayEvent)
        {
            AimedTarget = Cast<ARaidBossCharacterBase>(TriggerEventData->Target);
            
            ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

            PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Sevarog_E::EndAbilityCallback);
            PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Sevarog_E::EndAbilityCallback);
            PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Sevarog_E::EndAbilityCallback);
            PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Sevarog_E::EndAbilityCallback);

            WaitGameplayEvent->EventReceived.AddDynamic(this, &UAbility_Sevarog_E::AttackPointCallback);

            PlayMontageAndWait->ReadyForActivation();
            WaitGameplayEvent->ReadyForActivation();
        }
        else
        {
            EndAbilityCallback();
        }
    }
}

void UAbility_Sevarog_E::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateCancelAbility)
{
    bIsFirstTrigger = true;
    
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAbility_Sevarog_E::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
    AttackPointCount = 0;
    AimedTarget = nullptr;
    
    SpawnedIndicatorTasks.Reset();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Sevarog_E::SetIndicator()
{
    for (auto Indicator : TestIndicators)
    {
        if (Cast<AMonsterBase>(OwnerCharacter))
        {
            UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent =
                UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(
                    this, Indicator, AimedTarget, 0, FVector{1, SkillRadius, SkillRadius},
                    false, false);

            if (SpawnActorAndFollowParent == nullptr)
            {
                continue;
            }
            
            SpawnActorAndFollowParent->ReadyForActivation();
            SpawnedIndicatorTasks.Add(SpawnActorAndFollowParent);

            if (auto SpawnedIndicator = Cast<ACircleIndicator>(SpawnActorAndFollowParent->GetSpawnedActor()))
            {
                SpawnedIndicator->SetIndicatorValue(0.3);
                SpawnedIndicator->SetIndicatorColor(FColor::Red);
            }
        }
        else
        {
            UAT_SpawnActorAndFollowCamera* SpawnActorAndFollowCamera =
                UAT_SpawnActorAndFollowCamera::SpawnActorAndFollowCamera(
                    this, Indicator, OwnerCharacter, SkillRange, FVector{1, SkillRadius, SkillRadius},
                    bAttachLocationToParent, bFollowRotationToParent);

            if (SpawnActorAndFollowCamera == nullptr)
            {
                continue;
            }
            SpawnActorAndFollowCamera->ReadyForActivation();

            if (auto SpawnedIndicator = Cast<ASkillIndicator>(SpawnActorAndFollowCamera->GetSpawnedActor()))
            {
                SpawnedIndicator->SetIndicatorColor(FColor::Blue);
            }

            UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent =
                UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(
                    this, Indicator, OwnerCharacter, 0, {1, SkillRange, SkillRange}, true, false);

            if (SpawnActorAndFollowParent == nullptr)
            {
                continue;
            }
            
            SpawnActorAndFollowParent->ReadyForActivation();

            auto RangeIndicator = Cast<ACircleIndicator>(SpawnActorAndFollowParent->GetSpawnedActor());
            
            if (RangeIndicator)
            {
                RangeIndicator->SetIndicatorValue(0);
                RangeIndicator->SetIndicatorColor(FColor::Blue);
            }
        }
    }
}

void UAbility_Sevarog_E::AttackPointCallback(FGameplayEventData Payload)
{
    if (AttackPointCount == 0)
    {
        AttackPointCount++;

        if (Cast<AMonsterBase>(OwnerCharacter))
        {
            SetIndicator();
        }

        if (AimedTarget)
        {
            AttackLocation = GetFloorUnderTheTarget(AimedTarget);
        }
        else
        {
            AttackLocation = GetFloorAtCameraTracingLocation();
        }
    }
    else if (AttackPointCount == 1)
    {
        AttackPointCount++;
        URaidBossLibrary::SpawnEmitterAtLocation(GetWorld(), Particle, {FRotator::ZeroRotator, AttackLocation, {1.5, 1.5, 1.5}});
    }
    else
    {
        for (auto Element : SpawnedIndicatorTasks)
        {
            Element->ExternalCancel();
        }
        
        AttackPointCount = 0;
        
        AttackTargetsInAttackLocation();
    }
}

void UAbility_Sevarog_E::EndAbilityCallback()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

FVector UAbility_Sevarog_E::GetFloorUnderTheTarget(const AActor* Target) const
{
    FVector FloorOfTarget = URaidBossLibrary::GetFloorLocation(this, Target->GetActorLocation(), 3000);

    return FloorOfTarget;
}

FVector UAbility_Sevarog_E::GetFloorAtCameraTracingLocation() const
{
    FHitResult  HitResult;
    FVector     TraceEndLocation;
    bool        bTracingComplete =
        URaidBossLibrary::LineTraceForObjectFromCamera(this, OwnerCharacter,
                                                       {UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
                                                       {}, HitResult, TraceEndLocation, true, SkillRange);
    FVector     FloorOfTarget;

    if (bTracingComplete)
    {
        FloorOfTarget = URaidBossLibrary::GetFloorLocation(this, HitResult.Location);
    }
    else
    {
        FloorOfTarget = URaidBossLibrary::GetFloorLocation(this, TraceEndLocation);
    }

    return FloorOfTarget;
}

void UAbility_Sevarog_E::AttackTargetsInAttackLocation() const
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

    TArray<AActor*> TargetActors = URaidBossLibrary::GetActorsInAngle(this, AttackLocation, {1, 0, 0}, SkillRadius,
                                                                      FilterClasses, {}, 360, false);

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
