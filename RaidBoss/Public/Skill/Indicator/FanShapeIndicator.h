// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Indicator/SkillIndicator.h"
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
	
	virtual void	Tick(float DeltaSeconds) override;
	
protected:
	virtual void	BeginPlay() override;
	
public:

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	float	GetAngle() const { return Angle; }
	
	void	SetIndicatorValue(float InAngle) { Angle = InAngle; }
	void	SetAngle(float InAngle) { Angle = InAngle; }
	
protected:
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Indicator")
	TObjectPtr<UMaterialInstanceDynamic>	IndicatorInstance;
	
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
	float	Angle = 0;
};
