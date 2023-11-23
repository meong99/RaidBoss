// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Potion UMETA(DisplayName = "Potion"),
};

UCLASS()
class RAIDBOSS_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	EItemType ItemType;
};
