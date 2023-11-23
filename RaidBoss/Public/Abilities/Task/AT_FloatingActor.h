﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_FloatingActor.generated.h"

class ARaidBossCharacterBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAT_FloatingActor : public UAbilityTask
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="FloatingActor",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_FloatingActor* CreateFloatingActorTask(UGameplayAbility* OwningAbility, float InHeight, float InFloatingSpeed,
		float InFloatingTime, float InFallingSpeed);

	virtual void ExternalCancel() override;
	
protected:
	virtual void TickTask(float DeltaTime) override;
	
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	void	InterpolateDestinationXY();
protected:
	/*
	 *	Constant Value
	 */
	
	float Height;
	float FloatingSpeed;
	float FloatingTime;
	float ActivateTime = 0.f;
	float FallingSpeed;
	FVector	Destination;

	UPROPERTY()
	TObjectPtr<ARaidBossCharacterBase>	OwnerCharacter;
	
	/*
	 *	Changed in Initialize
	 */

	/*
	 *	Changed in every cycle
	 */

};
