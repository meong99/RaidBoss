// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillIndicator.generated.h"

UCLASS()
class RAIDBOSS_API ASkillIndicator : public AActor
{
	GENERATED_BODY()

public:
	ASkillIndicator();

	virtual void Tick(float DeltaTime) override;

	void	SetIndicatorColor(FColor InColor) { IndicatorColor = InColor; }
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill Indicator")
	TObjectPtr<UDecalComponent>	IndicatorDecal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill Indicator")
	FColor IndicatorColor = FColor::Blue;
};
