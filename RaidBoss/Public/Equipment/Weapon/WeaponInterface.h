// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Equipment/EquipmentType.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

class AWeapon;
class URaidBossAbilitySystemComponent;
class URaidBossAbilityBase;

UINTERFACE()
class UWeaponInterface : public UInterface
{
    GENERATED_BODY()
};

class RAIDBOSS_API IWeaponInterface
{
    GENERATED_BODY()

public:
    virtual bool                Attack() = 0;
    virtual AWeapon*            GetCurrentObject() = 0;
    virtual FWeaponKey          GetWeaponKey() const = 0;
    virtual const FWeaponData&  GetWeaponData() const = 0;
};
