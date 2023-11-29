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

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
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

protected:
	
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Base")
	TSubclassOf<UGameplayEffect>	EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	float	SkillRange = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	float	SkillCoolTime = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Base")
	FGameplayTagContainer	CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer	TempCooldownTags;
	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	FRaidBossSkillInfo	SkillInfo;
};
