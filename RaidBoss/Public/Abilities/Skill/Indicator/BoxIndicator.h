// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/SkillIndicator.h"
#include "BoxIndicator.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API ABoxIndicator : public ASkillIndicator
{
	GENERATED_BODY()
	
public:
	ABoxIndicator();
	
	virtual void	Tick(float DeltaSeconds) override;
	
protected:
	virtual void	BeginPlay() override;
	
public:
	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	float	GetLineThickness() const { return LineThickness; }
	
	void	SetIndicatorValue(float InLineThickness) { LineThickness = InLineThickness; }
	void	SetLineThickness(float InLineThickness) { LineThickness = InLineThickness; }
	
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
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
	float	LineThickness = 0.05;
};
