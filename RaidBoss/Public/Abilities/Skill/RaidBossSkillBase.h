#pragma once

#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossSkillType.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "RaidBossSkillBase.generated.h"

class ASkillIndicator;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class RAIDBOSS_API URaidBossSkillBase : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void	EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void	ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

/*
 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill")
	virtual float				GetSkillRange() const { return SkillRange; }
	const FRaidBossSkillInfo&	GetSkillInfo() const { return SkillInfo; };
	
protected:
	virtual void	SetIndicator() {}
	
	void	IncreaseSkillLevel();
	
	void	DecreaseSkillLevel();
	
	bool	CanLevelIncrease();
	
	bool	CanLevelDecrease();

	void	NotifySkillLevelChanged();
	
	bool	IsTargetInRangeXY(AActor* Target, float Range) const;
	
	bool	IsTargetInAngleXY(FVector StandardVector, FVector TargetVector, float MaxAngle) const;

	ESkillRequestType					GetRequestType(const FGameplayTagContainer* InstigatorTags) const;

	FGameplayAbilityTargetDataHandle	CreateAbilityTargetDataFromActor(AActor* Target) const;
	
	UAbilityTask_PlayMontageAndWait*	CreatePlayMontageAndWaitTask(int32 MontageIndex = 0, float Rate = 1, FName StartSection = NAME_None);
	
	UAbilityTask_WaitGameplayEvent*		CreateWaitGameplayEventTask(FGameplayTag EventTag, bool OnlyTriggerOnce = true);

protected:
	
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Base")
	TSubclassOf<UGameplayEffect>			EffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Indicator")
	TSubclassOf<ASkillIndicator>			SkillIndicatorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Indicator")
	TArray<TSubclassOf<ASkillIndicator>>	TestIndicators;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Skill Base")
	FGameplayTagContainer	CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer	TempCooldownTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Indicator")
	float	AdditiveIndicatorLocation = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Indicator")
	bool	bAttachLocationToParent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Indicator")
	bool	bFollowRotationToParent;

	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Skill Range")
	float	SkillRange = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Range")
	float	SkillSideRange = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Range")
	float	SkillRadius = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Range")
	float	SkillAngle = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	float	SkillDamageRate = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	float	SkillCoolTime = 1;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill Base")
	FRaidBossSkillInfo	SkillInfo;
};
