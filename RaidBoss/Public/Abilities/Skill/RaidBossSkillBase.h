#pragma once

#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossSkillType.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "RaidBossSkillBase.generated.h"

class ARaidBossCharacterBase;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class RAIDBOSS_API URaidBossSkillBase : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	URaidBossSkillBase();

/*
 *	----------- Overrided
 */
/*
 *	----------- Binded by Delegate
 */
/*
 *	----------- Other Method
 */
protected:
	FGameplayAbilityTargetDataHandle	CreateAbilityTargetDataFromActor(AActor* Target) const;
	UAbilityTask_PlayMontageAndWait*	CreatePlayMontageAndWaitTask(int32 MontageIndex = 0);
	UAbilityTask_WaitGameplayEvent*		CreateWaitGameplayEventTask(FGameplayTag EventTag, bool OnlyTriggerOnce = true);
	bool								IsTargetInRangeXY(AActor* Target, float Range);
	bool								IsTargetInAngleXY(FVector StandardVector, FVector TargetVector, float MaxAngle);
	void								BlockOwnerCharacterMovement();
	void								ReleaseOwnerCharacterMovement();
	
/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	const FRaidBossSkillInfo&	GetSkillInfo() const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	bool	IncreaseSkillLevel();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	bool	DecreaseSkillLevel();
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Skill Base")
	FRaidBossSkillInfo					SkillInfo;
};
