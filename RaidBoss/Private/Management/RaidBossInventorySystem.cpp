#include "Management/RaidBossInventorySystem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/RaidBossInventoryWidget.h"

URaidBossInventorySystem::URaidBossInventorySystem(const FObjectInitializer& Initializer)
{
	EquipItemInfo.SetNum(MaximumItemAmount);
	ConsumableItemInfo.SetNum(MaximumItemAmount);
}

void URaidBossInventorySystem::SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget)
{
	if (InInventoryWidget)
	{
		InventoryWidget = InInventoryWidget;
		InventoryWidget->SetWeakInventorySystem(this);
		InventoryWidget->CreateSlot();
	}
}

void URaidBossInventorySystem::ToggleInventoryWidget() const
{
	ARaidBossPlayerControllerBase* PlayerController = GetRaidBossPlayerControllerBase();
	
	if (PlayerController && InventoryWidget && InventoryWidget->IsInViewport() == false)
	{
		FInputModeGameAndUI InputMode;
		
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InventoryWidget->AddToViewportWithTracking();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else if (PlayerController && InventoryWidget)
	{
		FInputModeGameOnly	InputMode;
		int32				RemainingWidgets = 0;
		
		InventoryWidget->RemoveWidget(InventoryWidget, RemainingWidgets);
		if (RemainingWidgets == 0)
		{
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void URaidBossInventorySystem::AddNewItem(const TSubclassOf<URaidBossItemBase> NewItem)
{
	URaidBossItemBase* ItemCDO = NewItem ? Cast<URaidBossItemBase>(NewItem->GetDefaultObject()) : nullptr;
	
	if (ItemCDO == nullptr || IsInventoryFull(ItemCDO->GetItemCategory()))
	{
		return;
	}
	
	switch (ItemCDO->GetItemCategory())
	{
		case EITemCategory::Equip		: AddNewEquipItem(ItemCDO);			break;
		case EITemCategory::Consumable	: AddNewConsumableItem(ItemCDO);	break;
		default: break;
	}
}

void URaidBossInventorySystem::RemoveItem(EITemCategory ItemCategory, int32 Index)
{
	switch (ItemCategory)
	{
		case EITemCategory::Equip :
		{
			EquipItemInfo[Index].Reset();
			DecreaseCurrentItemCount(EITemCategory::Equip);
			break;
		}
		case EITemCategory::Consumable :
		{
			ConsumableItemInfo[Index].Reset();
			DecreaseCurrentItemCount(EITemCategory::Consumable);
			break;
		}
		default: break;
	}
}

void URaidBossInventorySystem::DecreaseItemAmount(EITemCategory ItemCategory, int32 Index)
{
	bool bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		if (EquipItemInfo[Index].Amount > 1)
		{
			EquipItemInfo[Index].Amount--;
		}
		else
		{
			RemoveItem(EITemCategory::Equip, Index);
		}
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		if (ConsumableItemInfo[Index].Amount > 1)
		{
			ConsumableItemInfo[Index].Amount--;
		}
		else
		{
			RemoveItem(EITemCategory::Consumable, Index);
		}
	}
}

void URaidBossInventorySystem::CallEquipItem(int32 Index)
{
	EquipItem(Index);
}

void URaidBossInventorySystem::CallUnEquipItem(int32 Index)
{
	TSubclassOf<URaidBossItemBase> UnEquippedItem = UnEquipItem(Index);

	AddNewItem(UnEquippedItem);
}

int32 URaidBossInventorySystem::UseConsumableItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent	= GetRaidBossAbilitySystemComponent();
	FItemInformation*					ItemInfo = GetItemInfo(EITemCategory::Consumable, Index);
	
	if (AbilitySystemComponent && ItemInfo && InventoryWidget)
	{
		FGameplayAbilitySpec		Spec		= AbilitySystemComponent->BuildAbilitySpecFromClass(ItemInfo->ItemClass);
		FGameplayAbilitySpecHandle	SpecHandle	= AbilitySystemComponent->GiveAbility(Spec);
		
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		AbilitySystemComponent->ClearAbility(SpecHandle);
		
		DecreaseItemAmount(EITemCategory::Consumable, Index);
		return ItemInfo->Amount;
	}
	
	return 0;
}

const URaidBossItemBase* URaidBossInventorySystem::GetItemCDO(EITemCategory ItemCategory, int32 Index) const
{
	TSubclassOf<URaidBossItemBase>	TempItemClass;
	
	bool bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		TempItemClass = EquipItemInfo[Index].ItemClass;
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		TempItemClass = ConsumableItemInfo[Index].ItemClass;
	}

	if (TempItemClass)
	{
		return Cast<URaidBossItemBase>(TempItemClass->GetDefaultObject());
	}
	
	return nullptr;
}

const TArray<FItemInformation>& URaidBossInventorySystem::GetEquipItems() const
{
	return EquipItemInfo;
}

const TArray<FItemInformation>& URaidBossInventorySystem::GetConsumableItems() const
{
	return ConsumableItemInfo;
}

const URaidBossEquipmentItem* URaidBossInventorySystem::GetEquipmentItem(int32 Index) const
{
	FItemInformation	CopyItemInfo = EquipItemInfo[Index];
	
	return Cast<URaidBossEquipmentItem>(CopyItemInfo.GetItemCDO());

}

const URaidBossEquipmentItem* URaidBossInventorySystem::GetEquippedItem(int32 Index) const
{
	TSubclassOf<URaidBossItemBase>	ItemClass = EquippedItems.FindRef(Index);

	if (ItemClass)
	{
		return  Cast<const URaidBossEquipmentItem>(ItemClass->GetDefaultObject());
	}
	
	return nullptr;
}

const int32& URaidBossInventorySystem::GetGold() const
{
	return Gold;
}

int32 URaidBossInventorySystem::GetMaximumItemAmount() const
{
	return MaximumItemAmount;
}

int32 URaidBossInventorySystem::GetItemAmount(EITemCategory ItemCategory, int32 Index) const
{
	int32	ItemAmount = 0;
	
	switch (ItemCategory)
	{
		case EITemCategory::Consumable	: ItemAmount = ConsumableItemInfo[Index].Amount;	break;
		case EITemCategory::Equip		: ItemAmount = EquipItemInfo[Index].Amount;			break;
		default: break;
	}

	return ItemAmount;
}

bool URaidBossInventorySystem::IsInventoryFull(EITemCategory ItemCategory) const
{
	bool	bIsInventoryFull;
	
	switch (ItemCategory)
	{
		case EITemCategory::Equip		: bIsInventoryFull = (CurrentEquipItemCount >= MaximumItemAmount);		break;
		case EITemCategory::Consumable	: bIsInventoryFull = (CurrentConsumableItemCount >= MaximumItemAmount);	break;
				
		default : bIsInventoryFull = true; break;
	}

	return bIsInventoryFull;
}

void URaidBossInventorySystem::AddGold(int32 AdditionalGold)
{
	Gold += AdditionalGold;
}

void URaidBossInventorySystem::SubtractGold(int32 SubtractionGold)
{
	Gold -= SubtractionGold;
}

void URaidBossInventorySystem::ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory)
{
	switch (ItemCategory)
	{
		case EITemCategory::Equip		: SwapItems(EquipItemInfo, Index1, Index2);			break;
		case EITemCategory::Consumable	: SwapItems(ConsumableItemInfo, Index1, Index2);	break;
		default: break;
	}
}

TSubclassOf<URaidBossItemBase> URaidBossInventorySystem::UnEquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent	= GetRaidBossAbilitySystemComponent();
	TSubclassOf<URaidBossItemBase>		ItemClass = EquippedItems.FindRef(Index);
	
	if (AbilitySystemComponent && InventoryWidget)
	{
		FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(ItemClass);
		
		if (Spec)
		{
			AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
			AbilitySystemComponent->ClearAbility(Spec->Handle);
		}
		
		EquippedItems.Remove(Index);
	}

	return ItemClass;
}

void URaidBossInventorySystem::EquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent	= GetRaidBossAbilitySystemComponent();
	FItemInformation*				 EquipmentItem	= GetItemInfo(EITemCategory::Equip, Index);
	const URaidBossEquipmentItem*	 ItemCDO		= Cast<URaidBossEquipmentItem>(EquipmentItem->GetItemCDO());
	int32							 EquipType		= ItemCDO ? ItemCDO->GetEquipType() : static_cast<int32>(EEquipType::None);
	
	TSubclassOf<URaidBossItemBase> AddToInventory = UnEquipItem(EquipType);
	
	if (AbilitySystemComponent && EquipmentItem && ItemCDO && InventoryWidget)
	{
		FGameplayAbilitySpec		Spec		= AbilitySystemComponent->BuildAbilitySpecFromClass(EquipmentItem->ItemClass);
		FGameplayAbilitySpecHandle	SpecHandle	= AbilitySystemComponent->GiveAbility(Spec);
		
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		
		EquippedItems.Add(ItemCDO->GetEquipType(), EquipmentItem->ItemClass);
		DecreaseItemAmount(EITemCategory::Equip, Index);
	}
	
	AddNewItem(AddToInventory);
}

void URaidBossInventorySystem::AddNewEquipItem(const URaidBossItemBase* ItemCDO)
{
	int32 EmptyIndex = INDEX_NONE;
	
	for (int32 i = 0; i < EquipItemInfo.Num(); i++)
	{
		if (EquipItemInfo[i].IsEmpty() && EmptyIndex == INDEX_NONE)
		{
			EmptyIndex = i;
		}
		else if (EquipItemInfo[i].ItemClass == ItemCDO->GetClass())
		{
			EquipItemInfo[i].Amount++;
			
			return;
		}
	}
	if (EmptyIndex != INDEX_NONE)
	{
		EquipItemInfo[EmptyIndex].ItemClass = ItemCDO->GetClass();
		EquipItemInfo[EmptyIndex].Amount++;
		IncreaseCurrentItemCount(EITemCategory::Equip);
	}
}

void URaidBossInventorySystem::AddNewConsumableItem(const URaidBossItemBase* ItemCDO)
{
	int32 EmptyIndex = INDEX_NONE;
	
	for (int32 i = 0; i < ConsumableItemInfo.Num(); i++)
	{
		if (ConsumableItemInfo[i].IsEmpty() && EmptyIndex == INDEX_NONE)
		{
			EmptyIndex = i;
		}
		else if (ConsumableItemInfo[i].ItemClass == ItemCDO->GetClass())
		{
			ConsumableItemInfo[i].Amount++;
			
			return;
		}
	}
	if (EmptyIndex != INDEX_NONE)
	{
		ConsumableItemInfo[EmptyIndex].ItemClass = ItemCDO->GetClass();
		ConsumableItemInfo[EmptyIndex].Amount++;
		IncreaseCurrentItemCount(EITemCategory::Consumable);
	}
}

void URaidBossInventorySystem::SwapItems(TArray<FItemInformation>& Items, int32 Index1, int32 Index2)
{
	if (Items.IsValidIndex(Index1) && Items.IsValidIndex(Index2))
	{
		FItemInformation TmpItem = Items[Index1];
		Items[Index1] = Items[Index2];
		Items[Index2] = TmpItem;
	}
}

void URaidBossInventorySystem::IncreaseItemAmount(EITemCategory ItemCategory, int32 Index)
{
	bool bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		EquipItemInfo[Index].Amount++;
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		ConsumableItemInfo[Index].Amount++;
	}
}

void URaidBossInventorySystem::IncreaseCurrentItemCount(EITemCategory ItemCategory)
{
	switch (ItemCategory)
	{
		case EITemCategory::Consumable	: CurrentConsumableItemCount++; break;
		case EITemCategory::Equip		: CurrentEquipItemCount++;		break;
		default: break;
	}

	if (CurrentConsumableItemCount > MaximumItemAmount)
	{
		CurrentConsumableItemCount = MaximumItemAmount;
	}
	else if (CurrentEquipItemCount > MaximumItemAmount)
	{
		CurrentEquipItemCount = MaximumItemAmount;
	}
}

void URaidBossInventorySystem::DecreaseCurrentItemCount(EITemCategory ItemCategory)
{
	switch (ItemCategory)
    {
    	case EITemCategory::Consumable	: CurrentConsumableItemCount--; break;
    	case EITemCategory::Equip		: CurrentEquipItemCount--;		break;
    	default: break;
    }

    if (CurrentConsumableItemCount < 0)
    {
    	CurrentConsumableItemCount = 0;
    }
    else if (CurrentEquipItemCount < 0)
    {
    	CurrentEquipItemCount = 0;
    }
}

bool URaidBossInventorySystem::IsValidItemIndex(EITemCategory ItemCategory, int32 Index) const
{
	bool bIsValidItemIndex;

	if (ItemCategory == EITemCategory::Equip)
	{
		bIsValidItemIndex = (EquipItemInfo.IsValidIndex(Index) && EquipItemInfo[Index].IsEmpty() == false);
	}
	else if (ItemCategory == EITemCategory::Consumable)
	{
		bIsValidItemIndex = (ConsumableItemInfo.IsValidIndex(Index) && ConsumableItemInfo[Index].IsEmpty() == false);
		
	}
	else
	{
		bIsValidItemIndex = false;
	}

	return bIsValidItemIndex;
}

ARaidBossPlayerControllerBase* URaidBossInventorySystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

URaidBossAbilitySystemComponent* URaidBossInventorySystem::GetRaidBossAbilitySystemComponent() const
{
	ARaidBossPlayerControllerBase*		Controller = GetRaidBossPlayerControllerBase();
	
	return Controller ? Controller->GetRaidBossAbilitySystemComponent() : nullptr;
}

FItemInformation* URaidBossInventorySystem::GetItemInfo(EITemCategory ItemCategory, int32 Index)
{
	bool bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
		return &EquipItemInfo[Index];
	if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
		return &ConsumableItemInfo[Index];

	return nullptr;
}
