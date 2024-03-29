﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/EquipManagement.h"
#include "Character/RaidBossCharacterBase.h"
#include "Equipment/Weapon/Weapon.h"

UEquipManagement::UEquipManagement()
{
    PrimaryComponentTick.bCanEverTick = true;
    EquipmentFactory = CreateDefaultSubobject<UEquipmentFactory>("Equipment Factory");
}

IWeaponInterface* UEquipManagement::Equip(FWeaponKey InWeaponKey)
{
    ARaidBossCharacterBase* OwnerCharacter = Cast<ARaidBossCharacterBase>(GetOwner());

    if (OwnerCharacter)
    {
        IWeaponInterface* NewWeapon = EquipmentFactory->CreateProduct(InWeaponKey, OwnerCharacter);

        return NewWeapon;
    }

    return nullptr;
}

void UEquipManagement::UnEquip(IWeaponInterface* Weapon)
{
    AWeapon* WeaponInstance = Weapon ? Weapon->GetCurrentObject() : nullptr;

    if (WeaponInstance)
    {
        WeaponInstance->ClearWeaponData();
        WeaponInstance->Destroy();
    }
}
