// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Indicator/CircleIndicator.h"
#include "Components/DecalComponent.h"

ACircleIndicator::ACircleIndicator()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SetActorTickEnabled(false);
}

void ACircleIndicator::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    IndicatorInstance->SetScalarParameterValue(FName("InnerCircleRadius"), Radius);
    IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
}

void ACircleIndicator::BeginPlay()
{
    Super::BeginPlay();
    
    SetActorTickEnabled(true);
    
    IndicatorInstance = IndicatorDecal->CreateDynamicMaterialInstance();
}
