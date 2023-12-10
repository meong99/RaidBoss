// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_DashCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAT_DashCharacter : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="CreateDashCharacterTask",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_DashCharacter*	CreateDashCharacterTask(UGameplayAbility* OwningAbility, FVector InDirection, float InDashSpeed,
		float InDashDistance);

protected:
	virtual void	TickTask(float DeltaTime) override;
	
	virtual void	Activate() override;

	virtual void	OnDestroy(bool bInOwnerFinished) override;
	
protected:
/*
*	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
	
	//
	float	DashDistance = 1;
	
	float	DashSpeed = 1;
	
	float	OriginMovementSpeed;
	
	FVector	OriginLocation;

	FVector	Direction = {1, 0, 0};
	
/*
 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	
	//

};
