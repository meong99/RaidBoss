// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "Ability_Move.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Move : public URaidBossAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
