// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Sevarog_E.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Sevarog_E : public URaidBossSkillBase
{
	GENERATED_BODY()

public:
	UAbility_Sevarog_E();

	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void	CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
protected:
	virtual void	EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void	SetIndicator() override;
	
	UFUNCTION()
	void	AttackPointCallback(FGameplayEventData Payload);
	
	UFUNCTION()
	void	EndAbilityCallback();
	
	FVector	GetFloorUnderTheTarget(const AActor* Target) const;

	FVector	GetFloorAtCameraTracingLocation() const;

	void	AttackTargetsInAttackLocation() const;
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Sevarog E")
	TObjectPtr<UParticleSystem>	Particle;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY()
	TArray<UAbilityTask*>	SpawnedIndicatorTasks;
	
	UPROPERTY()
	const ACharacter*		AimedTarget;
	
	bool	bIsFirstTrigger = true;

	int		AttackPointCount = 0;

	FVector AttackLocation;
};
