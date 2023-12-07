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
	
	virtual void Tick(float DeltaSeconds) override;

	void	SetIndicatorValue(float InLineThickness) { LineThickness = InLineThickness; }
	
	float	GetLineThickness() const { return LineThickness; }
	
	void	SetLineThickness(float InLineThickness) { LineThickness = InLineThickness; }
	
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
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
	float	LineThickness = 0.05;
};
