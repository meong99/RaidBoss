#pragma once

#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossSkillType.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "RaidBossSkillBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

enum ESkillRequestType
{
	None,
	IncreaseSkillLevel,
	DecreaseSkillLevel
};

UCLASS()
class RAIDBOSS_API URaidBossSkillBase : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Base")
	const FRaidBossSkillInfo&	GetSkillInfo() const;
	
	
protected:
	bool	CanLevelIncrease();
	
	bool	CanLevelDecrease();
	
	void	IncreaseSkillLevel();
	
	void	DecreaseSkillLevel();

	ESkillRequestType		GetRequestType(const FGameplayTagContainer* InstigatorTags) const;

	void	NotifySkillLevelChanged();

	
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
