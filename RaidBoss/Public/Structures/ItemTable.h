// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/ItemDataAsset.h"
#include "ItemTable.generated.h"

/**
 * 
 */

class URaidBossAbilityBase;

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32			ItemKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UItemDataAsset*	ItemData;
};

UCLASS()
class RAIDBOSS_API UItemTable : public UDataTable
{
	GENERATED_BODY()
};
