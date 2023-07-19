#pragma once

#include "CoreMinimal.h"
#include "Abilities/SKill/RaidBossSkillBase.h"
#include "Util/RaidBossTickableGameObject.h"
#include "RaidBossDash.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossDash : public URaidBossSkillBase, public FRaidBossTickableGameObject
{
	GENERATED_BODY()
public:
	URaidBossDash();

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
	virtual void	Tick(float DeltaTime) override;
/*
*	----------- Binded by Delegate
*/
public:
    UFUNCTION()
    void	EndAbilityCallback();
/*
*	----------- Other Method
*/
	bool	ActivateTasks();
	FVector	GetDestination();
	FVector	InterpolateDestinationByTracing(FVector Start, FVector End);
/*
*	----------- Access
*/
/*
*	----------- Member Variables
 */
private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
	UPROPERTY(EditDefaultsOnly)
	float	DashRange = 500;
	FVector	DashDestination;
};
