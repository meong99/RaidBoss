#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "UObject/Object.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include <unordered_map>
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

	bool				IsEmpty() const { return (Amount <= 0); }
	void				Reset() { Amount = 0; ItemClass = nullptr; }
	URaidBossItemBase*	GetItemCDO()
	{
		return ItemClass ? Cast<URaidBossItemBase>(ItemClass->GetDefaultObject()) : nullptr;
	}
	
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
	void	AddNewItem(TSubclassOf<URaidBossItemBase> NewItem);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	RemoveItem(EITemCategory ItemCategory, int32 Index);
	
	void	CallEquipItem(int32 Index);
	void	CallUnEquipItem(int32 Index);
	void	UseConsumableItem(int32 Index);
	void	ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory);
	
private:
	TSubclassOf<URaidBossItemBase>		UnEquipItem(int32 Index);
	void								EquipItem(int32 Index);
	void								AddNewEquipItem(URaidBossItemBase* ItemCDO);
	void								AddNewConsumableItem(URaidBossItemBase* ItemCDO);
	void								SwapItems(TArray<FItemInfomation>& Items, int32 Index1, int32 Index2);
	void								IncreaseItemAmount(EITemCategory ItemCategory, int32 Index);
	void								DecreaseItemAmount(EITemCategory ItemCategory, int32 Index);
	void								IncreaseCurrentItemCount(EITemCategory ItemCategory);
	void								DecreaseCurrentItemCount(EITemCategory ItemCategory);
	bool								IsValidItemIndex(EITemCategory ItemCategory, int32 Index) const;
	bool								IsInventoryFull(EITemCategory ItemCategory);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerControllerBase() const;
	const URaidBossEquipmentItem*		GetEquipmentItem(int32 Index) const;
	const URaidBossItemBase*			GetItemCDO(EITemCategory ItemCategory, int32 Index) const;
	const URaidBossEquipmentItem*		GetEquippedItem(int32 Index) const;
	int32								GetMaximumItemAmount() const;
	int32								GetItemAmount(EITemCategory ItemCategory, int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget);

private:
	FItemInfomation*					GetItemInfo(EITemCategory ItemCategory, int32 Index);
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	TObjectPtr<URaidBossInventoryWidget>	InventoryWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Inventory System")
	int32 MaximumItemAmount = 30;

private:
	TArray<FItemInfomation>						EquipItemInfo;
	TArray<FItemInfomation>						ConsumableItemInfo;
	TMap<int32, TSubclassOf<URaidBossItemBase>>	EquippedItemInfo;

	int32	CurrentEquipItemCount = 0;
	int32	CurrentConsumableItemCount = 0;
};
