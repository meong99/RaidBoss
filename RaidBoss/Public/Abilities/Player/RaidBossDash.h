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

	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void	Tick(float DeltaTime) override;

	
protected:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
private:
    UFUNCTION()
    void	EndAbilityCallback();
	bool	ActivateTasks();
	FVector	GetDestination();
	FVector	InterpolateDestinationByTracing(FVector Start, FVector End) const;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss")
	float	DashRange = 500;
	FVector	DashDestination;
};
