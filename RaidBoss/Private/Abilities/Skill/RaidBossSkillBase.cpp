#include "Abilities/Skill/RaidBossSkillBase.h"
#include "AbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Management/RaidBossGameplayTags.h"

bool URaidBossSkillBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags,
                                            FGameplayTagContainer* OptionalRelevantTags) const
{
    bool    bHasValidOwner = OwnerCharacter != nullptr;
    bool    bIsItForLeveling = GetRequestType(SourceTags) == ESkillRequestType::IncreaseSkillLevel ||
                               GetRequestType(SourceTags) == ESkillRequestType::DecreaseSkillLevel;
    bool    bIsItLearned = bIsItForLeveling == true || SkillInfo.SkillLevel > 0;
    bool    bCanActivateAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

    return bIsItLearned && bCanActivateAbility && bHasValidOwner;
}

void URaidBossSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
    ESkillRequestType RequestType = GetRequestType(&TriggerEventData->InstigatorTags);

    switch (RequestType)
    {
    case ESkillRequestType::IncreaseSkillLevel:
        {
            IncreaseSkillLevel();
            break;
        }
    case ESkillRequestType::DecreaseSkillLevel:
        {
            DecreaseSkillLevel();
            break;
        }
    default:
        break;
    }
}

void URaidBossSkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
    if (OwnerCharacter)
    {
        OwnerCharacter->SetIsMovementBlocked(false);
        OwnerCharacter->SetCanActivateNormalAttack(true);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URaidBossSkillBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo) const
{
    UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();

    if (CooldownGE)
    {
        FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass());

        EffectSpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
        EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().CoolDown, SkillCoolTime);

        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle);
    }
}

const FGameplayTagContainer* URaidBossSkillBase::GetCooldownTags() const
{
    FGameplayTagContainer*          MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
    const FGameplayTagContainer*    ParentTags = Super::GetCooldownTags();
    
    MutableTags->Reset();

    if (ParentTags)
    {
        MutableTags->AppendTags(*ParentTags);
    }
    
    MutableTags->AppendTags(CooldownTags);

    return MutableTags;
}

void URaidBossSkillBase::IncreaseSkillLevel()
{
    if (CanLevelIncrease())
    {
        SkillInfo.SkillLevel++;
        NotifySkillLevelChanged();
    }
}

void URaidBossSkillBase::DecreaseSkillLevel()
{
    if (CanLevelDecrease())
    {
        SkillInfo.SkillLevel--;
        NotifySkillLevelChanged();
    }
}

bool URaidBossSkillBase::CanLevelIncrease()
{
    if (SkillInfo.SkillLevel < SkillInfo.MaximumSkillLevel)
    {
        return true;
    }

    return false;
}

bool URaidBossSkillBase::CanLevelDecrease()
{
    if (SkillInfo.SkillLevel > SkillInfo.MinimumSkillLevel)
    {
        return true;
    }

    return false;
}

void URaidBossSkillBase::NotifySkillLevelChanged()
{
    if (OwnerCharacter)
    {
        OwnerCharacter->NotifySkillLevelChanged.Broadcast(GetAbilityTriggerTag(), SkillInfo.SkillLevel);
    }
}

bool URaidBossSkillBase::IsTargetInRangeXY(AActor* Target, float Range) const
{
    if (IsValid(Target) == false || IsValid(OwnerCharacter) == false)
    {
        return false;
    }

    FVector OwningActorLocation = OwnerCharacter->GetActorLocation();
    FVector EnermyActorLocation = Target->GetActorLocation();

    if (FVector::DistXY(OwningActorLocation, EnermyActorLocation) < Range)
    {
        return true;
    }

    return false;
}

bool URaidBossSkillBase::IsTargetInAngleXY(FVector StandardVector, FVector TargetVector, float MaxAngle) const
{
    StandardVector.Z = 0;
    TargetVector.Z = 0;
    
    StandardVector.Normalize();
    TargetVector.Normalize();

    float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(StandardVector, TargetVector)));

    if (Angle <= MaxAngle)
    {
        return true;
    }

    return false;
}

ESkillRequestType URaidBossSkillBase::GetRequestType(const FGameplayTagContainer* InstigatorTags) const
{
    if (InstigatorTags && InstigatorTags->IsValid())
    {
        const RaidBossGameplayTags& AllTags = RaidBossGameplayTags::Get();

        if (InstigatorTags->HasTag(AllTags.Event_Skill_IncreaseLevel))
        {
            return ESkillRequestType::IncreaseSkillLevel;
        }

        if (InstigatorTags->HasTag(AllTags.Event_Skill_DecreaseLevel))
        {
            return ESkillRequestType::DecreaseSkillLevel;
        }
    }

    return None;
}

FGameplayAbilityTargetDataHandle URaidBossSkillBase::CreateAbilityTargetDataFromActor(AActor* Target) const
{
    FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
    
    NewData->TargetActorArray.Add(Target);

    FGameplayAbilityTargetDataHandle Handle(NewData);

    return Handle;
}

UAbilityTask_PlayMontageAndWait* URaidBossSkillBase::CreatePlayMontageAndWaitTask(
    int32 MontageIndex, float Rate, FName StartSection)
{
    if (Montages.IsValidIndex(MontageIndex) == false)
    {
        return nullptr;
    }

    UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, FName(GetName()), Montages[MontageIndex], Rate, StartSection);

    return PlayMontageTask;
}

UAbilityTask_WaitGameplayEvent* URaidBossSkillBase::CreateWaitGameplayEventTask(
    FGameplayTag EventTag, bool OnlyTriggerOnce)
{
    UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this, EventTag, nullptr, OnlyTriggerOnce);
    
    return WaitEventTask;
}
