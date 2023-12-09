// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSkillBase.h"
#include "Ability_RangeAttack.generated.h"

class AWeapon;

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_RangeAttack : public URaidBossSkillBase
{
	GENERATED_BODY()
	
public:
	UAbility_RangeAttack();

	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void	OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void	EndAbilityCallBack();
	
	UFUNCTION()
	void	EventReceived(FGameplayEventData Payload);

	void	SetTracePointsFromCamera(OUT FVector& OutStartPoint,OUT FVector& OutEndPoint);
	
	void	SetTracePointsFromWeapon(OUT FVector& OutStartPoint,OUT FVector& OutEndPoint, const TArray<FHitResult>& HitResultRef);
	
	bool	StartTracingFromCamera(const FVector& StartPoint, const FVector& EndPoint, OUT TArray<FHitResult>& OutHitResults);
	
	bool	StartTracingFromWeapon(const FVector& StartPoint, const FVector& EndPoint, OUT AActor** OutTargetActor);
	
	void	ApplyEffectToTarget(AActor* TargetActor);

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	TWeakObjectPtr<AWeapon>	CurrentWeapon;
};
