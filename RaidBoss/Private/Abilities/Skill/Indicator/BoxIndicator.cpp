// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Indicator/BoxIndicator.h"
#include "Components/DecalComponent.h"

ABoxIndicator::ABoxIndicator()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void ABoxIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	IndicatorInstance->SetScalarParameterValue(FName("LineThickness"), LineThickness);
	IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
}

void ABoxIndicator::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
	IndicatorInstance = IndicatorDecal->CreateDynamicMaterialInstance();
}