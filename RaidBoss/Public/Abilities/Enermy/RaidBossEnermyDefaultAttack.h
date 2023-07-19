#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "RaidBossEnermyDefaultAttack.generated.h"

class ARaidBossPlayerBase;

UCLASS()
class RAIDBOSS_API URaidBossEnermyDefaultAttack : public URaidBossSkillBase
{
	GENERATED_BODY()

public:
	URaidBossEnermyDefaultAttack();

/*
 *	----------- Overrided
 */
public:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
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
	TArray<ARaidBossPlayerBase*>	SelectTargets();
	void							ApplyEffecsToTargets(const TArray<ARaidBossPlayerBase*>& TargetArr);
/*
 *	----------- Access
 */
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
};
