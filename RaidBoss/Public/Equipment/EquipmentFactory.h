// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Equipment/EquipmentType.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "EquipmentFactory.generated.h"

class ARaidBossCharacterBase;
class AWeapon;
class IWeaponInterface;


UCLASS()
class RAIDBOSS_API UEquipmentFactory : public UObject
{
	GENERATED_BODY()

public:
	IWeaponInterface*	CreateProduct(FWeaponKey InWeaponKey, ARaidBossCharacterBase* CharacterBase);
};
