// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "Ability_MeleeAttack.generated.h"

#define ComboInitValue 0

class UAbilityTask_WaitGameplayEvent;
class AWeapon;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_MeleeAttack : public URaidBossAbilityBase
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void	NotifyMontageCanceledCallBack();
	UFUNCTION()
	void	NotifyAttackPointComesCallBack(FGameplayEventData Payload);
	UFUNCTION()
	void	NotifyComboResetCallBack(FGameplayEventData Payload);
	UFUNCTION()
	void	NotifyCanActivateNextAttackCallBack(FGameplayEventData Payload);
	
	void	SetTracePoints(OUT FVector& OutStartPoint,OUT FVector& OutEndPoint);
	bool	StartTracing(const FVector& StartPoint, const FVector& EndPoint, OUT TArray<FHitResult>& OutHitResults);
	void	ApplyEffectToTarget();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32	MaximumCombo = 3;
	TObjectPtr<AWeapon>			CurrentWeapon;
	
	int32	CurrentCombo = ComboInitValue;
	TArray<TObjectPtr<AActor>>	HitActors;
	bool	bCanActivateNextAttack = true;
};
