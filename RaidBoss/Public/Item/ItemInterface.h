// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

struct FGameplayTagContainer;
class URaidBossAbilitySystemComponent;
class URaidBossAbilityBase;
// This class does not need to be modified.
UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

UENUM()
enum class EItemAnimType
{
	EquipAnim UMETA(DisplayName = "EquipAnim"),
	UnEquipAnim UMETA(DisplayName = "UnEquipAnim"),
	UseItemAnim UMETA(DisplayName = "UseItemAnim"),
};

class RAIDBOSS_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual void	InitItemData(int32 ItemKey) = 0;
	virtual bool	UseItem(URaidBossAbilitySystemComponent* ASC) = 0;
	virtual void	AddActivationTags(FGameplayTagContainer Tags) = 0;
	virtual bool	PlayItemAnimMontage(EItemAnimType AnimType) = 0;
	
	virtual		TArray<TSubclassOf<URaidBossAbilityBase>>	GetAbilities() = 0;
	virtual		EItemType	GetItemType() = 0;
	virtual		AActor*		GetCurrentObject() = 0;
	virtual		int32		GetItemKey() = 0;
};
