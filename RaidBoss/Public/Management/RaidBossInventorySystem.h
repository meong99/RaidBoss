#pragma once

#include "UObject/Object.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "RaidBossInventorySystem.generated.h"

class ARaidBossPlayerControllerBase;
class URaidBossInventoryWidget;
class URaidBossEquipmentItem;

USTRUCT()
struct FItemInformation
{
	GENERATED_BODY()

	bool						IsEmpty() const { return (Amount <= 0); }
	void						Reset() { Amount = 0; ItemClass = nullptr; }
	const URaidBossItemBase*	GetItemCDO() const
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
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	ToggleInventoryWidget() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory System")
	void	AddNewItem(const TSubclassOf<URaidBossItemBase> NewItem);
	void	RemoveItem(EITemCategory ItemCategory, int32 Index);
	void	DecreaseItemAmount(EITemCategory ItemCategory, int32 Index);
	
	void	CallEquipItem(int32 Index);
	void	CallUnEquipItem(int32 Index);
	int32	UseConsumableItem(int32 Index);
	
	const URaidBossItemBase*		GetItemCDO(EITemCategory ItemCategory, int32 Index) const;
	const URaidBossEquipmentItem*	GetEquipmentItem(int32 Index) const;
	const URaidBossEquipmentItem*	GetEquippedItem(int32 Index) const;
	const int32&					GetGold() const;
	const TArray<FItemInformation>&	GetEquipItems() const;
	const TArray<FItemInformation>&	GetConsumableItems() const;
	int32							GetMaximumItemAmount() const;
	int32							GetItemAmount(EITemCategory ItemCategory, int32 Index) const;
	bool							IsInventoryFull(EITemCategory ItemCategory) const;

	void	AddGold(int32 AdditionalGold);
	void	SubtractGold(int32 SubtractionGold);
	void	ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory);
	
private:
	TSubclassOf<URaidBossItemBase>	UnEquipItem(int32 Index);
	void							EquipItem(int32 Index);
	void							AddNewEquipItem(const URaidBossItemBase* ItemCDO);
	void							AddNewConsumableItem(const URaidBossItemBase* ItemCDO);
	
	void	SwapItems(TArray<FItemInformation>& Items, int32 Index1, int32 Index2);
	void	IncreaseItemAmount(EITemCategory ItemCategory, int32 Index);
	void	IncreaseCurrentItemCount(EITemCategory ItemCategory);
	void	DecreaseCurrentItemCount(EITemCategory ItemCategory);
	bool	IsValidItemIndex(EITemCategory ItemCategory, int32 Index) const;

	ARaidBossPlayerControllerBase*		GetRaidBossPlayerControllerBase() const;
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	FItemInformation*					GetItemInfo(EITemCategory ItemCategory, int32 Index);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System", meta=(AllowPrivateAccess))
	TObjectPtr<URaidBossInventoryWidget>	InventoryWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Inventory System", meta=(AllowPrivateAccess))
	int32	MaximumItemAmount = 30;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Inventory System", meta=(AllowPrivateAccess))
	int32	Gold = 0;
	
	TArray<FItemInformation>						EquipItemInfo;
	TArray<FItemInformation>						ConsumableItemInfo;
	TMap<int32, TSubclassOf<URaidBossItemBase>>	EquippedItems;

	int32	CurrentEquipItemCount = 0;
	int32	CurrentConsumableItemCount = 0;
};
