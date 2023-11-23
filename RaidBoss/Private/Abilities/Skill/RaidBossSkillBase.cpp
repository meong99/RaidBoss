#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Management/RaidBossGameplayTags.h"

bool URaidBossSkillBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bIsItForLeveling = GetRequestType(SourceTags) == ESkillRequestType::IncreaseSkillLevel ||
							GetRequestType(SourceTags) == ESkillRequestType::DecreaseSkillLevel;
	
	bool bIsItLearned = bIsItForLeveling == true ||
						SkillInfo.SkillLevel > 0;
	
	return bIsItLearned &&
		Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void URaidBossSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ESkillRequestType RequestType = GetRequestType(&TriggerEventData->InstigatorTags);

	switch (RequestType)
	{
	case ESkillRequestType::IncreaseSkillLevel :
	{
		IncreaseSkillLevel();
		break;
	}
	case ESkillRequestType::DecreaseSkillLevel :
	{
		DecreaseSkillLevel();
		break;
	}
	default:
		break;
	}
}

const FRaidBossSkillInfo& URaidBossSkillBase::GetSkillInfo() const
{
	return SkillInfo;
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

ESkillRequestType URaidBossSkillBase::GetRequestType(const FGameplayTagContainer* InstigatorTags) const
{
	if (InstigatorTags)
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

void URaidBossSkillBase::NotifySkillLevelChanged()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->NotifySkillLevelChanged.Broadcast(GetAbilityTriggerTag(), SkillInfo.SkillLevel);
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

FGameplayAbilityTargetDataHandle URaidBossSkillBase::CreateAbilityTargetDataFromActor(AActor* Target) const
{
	FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
	NewData->TargetActorArray.Add(Target);

	FGameplayAbilityTargetDataHandle	Handle(NewData);

	return Handle;
}

UAbilityTask_PlayMontageAndWait* URaidBossSkillBase::CreatePlayMontageAndWaitTask(int32 MontageIndex)
{
	if (Montages.IsValidIndex(MontageIndex) == false)
		return nullptr;
		
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, FName(GetName()), Montages[MontageIndex]);
	
	return PlayMontageTask;
}

UAbilityTask_WaitGameplayEvent* URaidBossSkillBase::CreateWaitGameplayEventTask(FGameplayTag EventTag, bool OnlyTriggerOnce)
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, OnlyTriggerOnce);
	return WaitEventTask;
}

bool URaidBossSkillBase::IsTargetInRangeXY(AActor* Target, float Range) const
{
	if (IsValid(Target) == false || IsValid(OwnerCharacter) == false)
		return false;

	FVector	OwningActorLocation = OwnerCharacter->GetActorLocation();
	FVector	EnermyActorLocation = Target->GetActorLocation();

	if (FVector::DistXY(OwningActorLocation, EnermyActorLocation) < Range)
		return true;

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
		return true;

	return false;
}

void URaidBossSkillBase::BlockOwnerCharacterMovement() const
{
	if (IsValid(OwnerCharacter) == true)
		OwnerCharacter->TurnOffCharacterStateBitMap(ECharacterState::CanMove);
}

void URaidBossSkillBase::ReleaseOwnerCharacterMovement() const
{
	if (IsValid(OwnerCharacter) == true)
		OwnerCharacter->TurnOnCharacterStateBitMap(ECharacterState::CanMove);
}
