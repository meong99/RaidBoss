// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "ItemData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<URaidBossItemBase> ItemClass;
};
