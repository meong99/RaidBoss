#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

const FRaidBossSkillInfo& URaidBossSkillBase::GetSkillInfo() const
{
	return SkillInfo;
}

bool URaidBossSkillBase::IncreaseSkillLevel()
{
	if (SkillInfo.SkillLevel + 1 <= SkillInfo.MaximumSkillLevel)
	{
		SkillInfo.SkillLevel++;

		return  true;
	}

	return false;
}

bool URaidBossSkillBase::DecreaseSkillLevel()
{
	if ( SkillInfo.SkillLevel - 1 >= SkillInfo.MinimumSkillLevel)
	{
		SkillInfo.SkillLevel--;
		return  true;
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
