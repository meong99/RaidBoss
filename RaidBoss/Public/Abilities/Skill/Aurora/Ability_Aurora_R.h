// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Aurora_R.generated.h"

class UAT_FloatingActor;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Aurora_R : public URaidBossSkillBase
{
	GENERATED_BODY()

public:
	UAbility_Aurora_R();
	
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void	CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
protected:

	virtual void	EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void	SetIndicator() override;
	
	UFUNCTION()
	void	AttackPointCallback(FGameplayEventData EventData);
	
	UFUNCTION()
	void	EndAbilityCallback();

protected:

	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora R")
	float	FloatingHeight = 200;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora R")
	float	FloatingMoveSpeed = 200;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora R")
	float	RisingSpeed = 200;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora R")
	float	FallingSpeed = 200;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora R")
	float	FloatingTime = 1.3;
	
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	float	OriginFlySpeed = 0;

	bool	bIsFirstTrigger = true;
};
