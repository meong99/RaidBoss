// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/EquipmentFactory.h"

#include "Character/RaidBossCharacterBase.h"
#include "Equipment/Weapon/Weapon.h"

IArmorInterface* UEquipmentFactory::CreateProduct(FArmorKey InArmorKey, ARaidBossCharacterBase* CharacterBase)
{
	return nullptr;
}

IWeaponInterface* UEquipmentFactory::CreateProduct(FWeaponKey InWeaponKey, ARaidBossCharacterBase* CharacterBase)
{
	AWeapon* NewWeapon = nullptr;

	if (CharacterBase)
	{
		FActorSpawnParameters ActorSpawnParameters;

		ActorSpawnParameters.Owner = CharacterBase;
		
		// NewWeapon = CharacterBase->GetWorld()->SpawnActor<AWeapon>(
		// 	AWeapon::StaticClass(), FTransform::Identity, ActorSpawnParameters);
		NewWeapon = CharacterBase->GetWorld()->SpawnActorDeferred<AWeapon>(
			AWeapon::StaticClass(), FTransform::Identity, CharacterBase);
		NewWeapon->LoadWeaponData(InWeaponKey);
	}

	return NewWeapon;
}
