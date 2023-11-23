// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "Equipment/EquipmentType.h"
#include "WeaponDataAsset.generated.h"

UCLASS()
class RAIDBOSS_API UWeaponDataAsset : public UItemDataAsset
{
GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	FWeaponData	WeaponData;
};
