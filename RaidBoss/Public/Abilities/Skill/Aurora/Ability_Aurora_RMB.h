// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Aurora_RMB.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Aurora_RMB : public URaidBossSkillBase
{
	GENERATED_BODY()
public:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void	EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void	EndAbilityCallback();	

	UFUNCTION()
	void	ReleaseMovementCallback(FGameplayEventData Payload);
	
	void	GetDirectionAndMontageSection(OUT FVector& OutLaunchDirection, OUT FName& OutStartSection);

	void	LaunchCharacter(FVector LaunchDirection);
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora RMB")
	float	LaunchPower = 500;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora RMB")
	float	LaunchArc = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora RMB")
	bool	bDrawDebugLine = false;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
