// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/EquipmentFactory.h"

#include "Character/RaidBossCharacterBase.h"
#include "Equipment/Weapon/Weapon.h"

IWeaponInterface* UEquipmentFactory::CreateProduct(FWeaponKey InWeaponKey, ARaidBossCharacterBase* CharacterBase)
{
    AWeapon* NewWeapon = nullptr;

    if (CharacterBase)
    {
        FActorSpawnParameters ActorSpawnParameters;

        ActorSpawnParameters.Owner = CharacterBase;
        NewWeapon = CharacterBase->GetWorld()->SpawnActorDeferred<AWeapon>(AWeapon::StaticClass(), FTransform::Identity, CharacterBase);
        
        NewWeapon->LoadWeaponData(InWeaponKey);
    }

    return NewWeapon;
}
