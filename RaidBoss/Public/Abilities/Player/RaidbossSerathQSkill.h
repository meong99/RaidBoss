#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "RaidbossSerathQSkill.generated.h"

class ARaidBossEnemyBase;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class RAIDBOSS_API URaidbossSerathQSkill : public URaidBossSkillBase
{
	GENERATED_BODY()
public:
	URaidbossSerathQSkill();
	
/*
 *	----------- Overrided
 */
public:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const override;
/*
 *	----------- Binded by Delegate
 */
public:
	UFUNCTION()
	void			EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void			EndAbilityCallback();
	
/*
 *	----------- Other Method
 */
protected:
	bool							ActivateTasks();
	TArray<ARaidBossEnemyBase*>	SelectTargets();
	void							ApplyEffecsToTargets(TArray<ARaidBossEnemyBase*> TargetArr);
	float							CalculateAdditialnalAttackPower();
	
/*
 *	----------- Access
 */
	
/*
 *	----------- Member Variables
 */
protected:
	float	Range = 1000;
	float	IncreaseRate = 20;
	
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss")
	TObjectPtr<UParticleSystem>					Particle;
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
};
