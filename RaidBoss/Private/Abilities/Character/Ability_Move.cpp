// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Character/Ability_Move.h"

#include "Character/RaidBossCharacterBase.h"

void UAbility_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (OwnerCharacter)
	{
		OwnerCharacter->AddMovementInput(OwnerCharacter->GetMovementDirection(), OwnerCharacter->GetMovementMagnitude());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
