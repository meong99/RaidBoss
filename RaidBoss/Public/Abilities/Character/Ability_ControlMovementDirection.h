// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "Ability_ControlMovementDirection.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_ControlMovementDirection : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	void	RotateToForward();
	
	void	RotateByMovementInput(FRotator NewRotator);

	void	RotateToForwardWithThreshold();

	FRotator	GetRotationByMovementInput() const;
	
	FRotator	GetAlignedRotationByForwardVector();
	
	void		GetMovementInput(OUT float& OutMoveForward, OUT float& OutMoveRight) const;

protected:
	
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid boss")
	float InterpSpeedAtAttack = 15;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid boss")
	float InterpSpeed = 8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid boss")
	float AlignSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid boss")
	float AlignThreshold = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid boss")
	float PlayLate = 1.0f;

	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	bool	bIsDuringAlign = false;
};
