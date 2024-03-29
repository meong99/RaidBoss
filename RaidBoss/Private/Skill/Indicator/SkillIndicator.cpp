﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Indicator/SkillIndicator.h"
#include "Components/DecalComponent.h"

ASkillIndicator::ASkillIndicator()
{
    IndicatorDecal = CreateDefaultSubobject<UDecalComponent>("IndicatorDecal");

    IndicatorDecal->SetRelativeRotation(FRotator(-90, 0, 0));
}
