// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Indicator/FanShapeIndicator.h"
#include "Components/DecalComponent.h"

AFanShapeIndicator::AFanShapeIndicator()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SetActorTickEnabled(false);
}

void AFanShapeIndicator::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    IndicatorInstance->SetScalarParameterValue(FName("Angle"), Angle);
    IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
}

void AFanShapeIndicator::BeginPlay()
{
    Super::BeginPlay();
    
    SetActorTickEnabled(true);
    
    IndicatorInstance = IndicatorDecal->CreateDynamicMaterialInstance();
}
