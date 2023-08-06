#pragma once

#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossSkillType.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "RaidBossSkillBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class RAIDBOSS_API URaidBossSkillBase : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	const FRaidBossSkillInfo&	GetSkillInfo() const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	bool	IncreaseSkillLevel();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	bool	DecreaseSkillLevel();
	
protected:
	FGameplayAbilityTargetDataHandle	CreateAbilityTargetDataFromActor(AActor* Target) const;
	UAbilityTask_PlayMontageAndWait*	CreatePlayMontageAndWaitTask(int32 MontageIndex = 0);
	UAbilityTask_WaitGameplayEvent*		CreateWaitGameplayEventTask(FGameplayTag EventTag, bool OnlyTriggerOnce = true);
	bool								IsTargetInRangeXY(AActor* Target, float Range) const;
	bool								IsTargetInAngleXY(FVector StandardVector, FVector TargetVector, float MaxAngle) const;
	void								BlockOwnerCharacterMovement() const;
	void								ReleaseOwnerCharacterMovement() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Skill Base")
	FRaidBossSkillInfo	SkillInfo;
};
