// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Sevarog_Q.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Sevarog_Q : public URaidBossSkillBase
{
	GENERATED_BODY()
public:
	UAbility_Sevarog_Q();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void	AttackPointCallback(FGameplayEventData Payload);
	
	UFUNCTION()
	void	EndAbilityCallback();

	virtual void	SetIndicator() override;

	TArray<AActor*>	StartTraceAndGetTargets();
protected:
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Sevarog E")
	bool	bDrawDebugLine = false;
	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
	UPROPERTY()
	TArray<UAbilityTask*>	SpawnedIndicatorTasks;
	
	bool	bIsFirstTrigger = true;
};
