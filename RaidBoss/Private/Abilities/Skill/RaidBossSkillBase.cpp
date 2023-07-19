#include "Abilities/Skill/RaidBossSkillBase.h"
#include "AbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

URaidBossSkillBase::URaidBossSkillBase()
{
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

bool URaidBossSkillBase::IsTargetInRangeXY(AActor* Target, float Range)
{
	if (IsValid(Target) == false || IsValid(OwnerCharacter) == false)
		return false;

	FVector	OwningActorLocation = OwnerCharacter->GetActorLocation();
	FVector	EnermyActorLocation = Target->GetActorLocation();

	if (FVector::DistXY(OwningActorLocation, EnermyActorLocation) < Range)
		return true;

	return false;
}

bool URaidBossSkillBase::IsTargetInAngleXY(FVector StandardVector, FVector TargetVector, float MaxAngle)
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

void URaidBossSkillBase::BlockOwnerCharacterMovement()
{
	if (IsValid(OwnerCharacter) == true)
		OwnerCharacter->TurnOffCharacterStateBitMap(ECharacterState::CanMove);
}

void URaidBossSkillBase::ReleaseOwnerCharacterMovement()
{
	if (IsValid(OwnerCharacter) == true)
		OwnerCharacter->TurnOnCharacterStateBitMap(ECharacterState::CanMove);
}

const FRaidBossSkillInfo& URaidBossSkillBase::GetSkillInfo() const
{
	return SkillInfo;
}

void URaidBossSkillBase::IncreaseSkillLevel()
{
	if (IsValid(OwnerCharacter) == true && OwnerCharacter->GetSkillPoint() > 0 &&
		SkillInfo.SkillLevel + 1 <= SkillInfo.MaximumSkillLevel)
	{
		SkillInfo.SkillLevel++;
		OwnerCharacter->DecreaseSkillPoint();
	}
}

void URaidBossSkillBase::DecreaseSkillLevel()
{
	if (IsValid(OwnerCharacter) == true && SkillInfo.SkillLevel - 1 >= SkillInfo.MinimumSkillLevel)
	{
		SkillInfo.SkillLevel--;
		OwnerCharacter->IncreaseSkillPoint();
	}
}
