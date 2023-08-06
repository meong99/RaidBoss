#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "RaidBossEnemyDefaultAttack.generated.h"

class ARaidBossPlayerBase;

UCLASS()
class RAIDBOSS_API URaidBossEnemyDefaultAttack : public URaidBossSkillBase
{
	GENERATED_BODY()

public:
	URaidBossEnemyDefaultAttack();

protected:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void	EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void	EndAbilityCallback();
	
	TArray<ARaidBossPlayerBase*>	SelectTargets();
	void							ApplyEffectsToTargets(const TArray<ARaidBossPlayerBase*>& TargetArr);

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
};
