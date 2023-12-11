// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Indicator/SkillIndicator.h"
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

    virtual void    Tick(float DeltaSeconds) override;

protected:
    virtual void    BeginPlay() override;

public:
    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    float   GetRadius() const { return Radius; }

    void    SetIndicatorValue(float InRadius) { Radius = InRadius; }
    void    SetRadius(float InRadius) { Radius = InRadius; }

protected:

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(BlueprintReadWrite, Category="Indicator")
    TObjectPtr<UMaterialInstanceDynamic>    IndicatorInstance;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
    float   Radius = 0;
};
