﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipment/EquipmentFactory.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "EquipManagement.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API UEquipManagement : public UActorComponent
{
    GENERATED_BODY()

public:
    UEquipManagement();

public:
    IWeaponInterface*   Equip(FWeaponKey InWeaponKey);

    void                UnEquip(IWeaponInterface* Weapon);

private:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    UEquipmentFactory*  EquipmentFactory;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
