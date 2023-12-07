// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/SkillIndicator.h"
#include "FanShapeIndicator.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API AFanShapeIndicator : public ASkillIndicator
{
	GENERATED_BODY()

public:
	AFanShapeIndicator();
	
	virtual void Tick(float DeltaSeconds) override;

	void	SetIndicatorValue(float InAngle) { Angle = InAngle; }
	
	float	GetAngle() const { return Angle; }
	
	void	SetAngle(float InAngle) { Angle = InAngle; }
	
protected:
	virtual void BeginPlay() override;
	
protected:
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Indicator")
	TObjectPtr<UMaterialInstanceDynamic>	IndicatorInstance;
	
	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
	float	Angle = 0;
};
