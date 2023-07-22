#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "UObject/Object.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "RaidBossInventorySystem.generated.h"

class URaidBossEquipmentWidget;
class URaidBossItemBase;
class URaidBossConsumableItem;
class URaidBossEquipmentItem;
class URaidBossInventoryWidget;
class ARaidBossPlayerControllerBase;

USTRUCT()
struct FItemInfomation
{
	GENERATED_BODY()

	bool	IsEmpty() const { return (Amount <= 0); }
	void	Reset() { Amount = 0; ItemClass = nullptr; }
	
	int32							Amount = 0;
	TSubclassOf<URaidBossItemBase>	ItemClass;
};

UCLASS(BlueprintType, Blueprintable)
class RAIDBOSS_API URaidBossInventorySystem : public UObject
{
	GENERATED_BODY()

public:
	URaidBossInventorySystem(const FObjectInitializer& Initializer);
/*
*	----------- Override
*/
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	ToggleInventoryWidget();
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	AddNewItem(const TSubclassOf<URaidBossItemBase>& NewItem);
	// void	AddNewItem(URaidBossItemBase* NewItem);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	RemoveItem(EITemCategory ItemCategory, int32 Index);
	
	void	CallEquipItem(int32 Index);
	void	CallUnEquipItem(int32 Index);
	void	UseConsumableItem(int32 Index);
	void	ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory);
	
private:
	void					AddNewEquipItem(URaidBossItemBase* ItemCDO);
	void					AddNewConsumableItem(URaidBossItemBase* ItemCDO);
	URaidBossEquipmentItem*	UnEquipItem(int32 Index);
	void					EquipItem(int32 Index);
	void					SwapItems(TArray<URaidBossItemBase*>& Items, int32 Index1, int32 Index2);
	void					IncreaseItemCount(EITemCategory ItemCategory, int32 Index);
	void					DecreaseItemCount(EITemCategory ItemCategory, int32 Index);
	bool					IsValidItemIndex(EITemCategory ItemCategory, int32 Index, bool DoCheckObject =  false);
	bool					IsInventoryFull(EITemCategory ItemCategory);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	ARaidBossPlayerControllerBase*			GetRaidBossPlayerControllerBase() const;
	URaidBossAbilitySystemComponent*		GetRaidBossAbilitySystemComponent() const;
	URaidBossItemBase*						GetItem(EITemCategory ItemCategory, int32 Index);
	const URaidBossEquipmentItem*			GetEquippedItem(int32 Index);
	int32									GetMaximumItemCount() const;

	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget);
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	TObjectPtr<URaidBossInventoryWidget>	InventoryWidget;
	// UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	// TArray<URaidBossItemBase*>				EquipItemObjects;
	// UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	// TArray<URaidBossItemBase*>				ConsumableItemObjects;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	TMap<int32, URaidBossEquipmentItem*>	EquippedItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	int32 MaximumItemCount = 30;

private:
	TArray<FItemInfomation>			EquipItemInfo;
	TArray<FItemInfomation>			ConsumableItemInfo;
	TMap<int32, FItemInfomation>	EquippedItemInfo;
};
