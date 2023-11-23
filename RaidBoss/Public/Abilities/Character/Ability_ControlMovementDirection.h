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

	FRotator	GetNewRotation() const;
	FRotator	GetAlignedRotation();
	void		GetMoveDirection(OUT float& OutMoveForward, OUT float& OutMoveRight) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InterpSpeed = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AlignSpeed = 250;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AlignThreshold = 0.7f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PlayLate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>	TurnRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>	TurnLeft;

	bool	bIsDuringAlign = false;
};
