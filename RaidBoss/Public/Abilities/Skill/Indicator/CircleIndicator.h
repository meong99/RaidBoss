// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/SkillIndicator.h"
#include "CircleIndicator.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API ACircleIndicator : public ASkillIndicator
{
	GENERATED_BODY()
	
public:
	ACircleIndicator();
	
	virtual void Tick(float DeltaSeconds) override;

	void	SetIndicatorValue(float InRadius) { Radius = InRadius; }
	
	float	GetRadius() const { return Radius; }
	
	void	SetRadius(float InRadius) { Radius = InRadius; }
	
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
	float	Radius = 0;
};
