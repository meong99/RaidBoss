#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "RaidBossSerathQSkill.generated.h"

class ARaidBossEnemyBase;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class RAIDBOSS_API URaidBossSerathQSkill : public URaidBossSkillBase
{
	GENERATED_BODY()
public:
	URaidBossSerathQSkill();

	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const override;
	UFUNCTION()
	void			EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void			EndAbilityCallback();

protected:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
	TArray<ARaidBossEnemyBase*>		SelectTargets();
	bool							ActivateTasks();
	void							ApplyEffectsToTargets(TArray<ARaidBossEnemyBase*> TargetArr);
	float							CalculateAdditionalAttackPower();

private:
	float	Range = 1000;
	float	IncreaseRate = 20;
	
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss")
	TObjectPtr<UParticleSystem>					Particle;
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
};
