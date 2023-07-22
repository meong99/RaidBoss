#include "Management/RaidBossInventorySystem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/RaidBossInventoryWidget.h"

URaidBossInventorySystem::URaidBossInventorySystem(const FObjectInitializer& Initializer)
{
	EquipItemInfo.SetNum(MaximumItemAmount);
	ConsumableItemInfo.SetNum(MaximumItemAmount);
}

void URaidBossInventorySystem::ToggleInventoryWidget()
{
	ARaidBossPlayerControllerBase* PlayerController;

	PlayerController = GetRaidBossPlayerControllerBase();
	
	if (PlayerController && InventoryWidget && InventoryWidget->IsInViewport() == false)
	{
		FInputModeGameAndUI InputMode;

		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InventoryWidget->AddToViewport();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else if (PlayerController && InventoryWidget)
	{
		FInputModeGameOnly InputMode;
		
		InventoryWidget->RemoveFromParent();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

void URaidBossInventorySystem::AddNewItem(TSubclassOf<URaidBossItemBase> NewItem)
{
	URaidBossItemBase* ItemCDO;
	
	ItemCDO = NewItem ? Cast<URaidBossItemBase>(NewItem->GetDefaultObject()) : nullptr;
	
	if (ItemCDO == nullptr || IsInventoryFull(ItemCDO->GetItemCategory()))
		return;
	
	switch (ItemCDO->GetItemCategory())
	{
		case EITemCategory::Equip		: AddNewEquipItem(ItemCDO);			break;
		case EITemCategory::Consumable	: AddNewConsumableItem(ItemCDO);	break;
		default: break;
	}

	if (InventoryWidget)
		InventoryWidget->UpdateInventorySlot();
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
	if (InventoryWidget)
		InventoryWidget->UpdateInventorySlot();
}

void URaidBossInventorySystem::CallEquipItem(int32 Index)
{
	EquipItem(Index);
}

void URaidBossInventorySystem::CallUnEquipItem(int32 Index)
{
	TSubclassOf<URaidBossItemBase>	UnEquippedItem;
	
	UnEquippedItem = UnEquipItem(Index);

	AddNewItem(UnEquippedItem);
}

void URaidBossInventorySystem::EquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	FItemInfomation*					EquipmentItem;
	TSubclassOf<URaidBossItemBase>		AddToInventory;
	URaidBossEquipmentItem*				ItemCDO;
	FGameplayAbilitySpec				Spec;
	FGameplayAbilitySpecHandle			SpecHandle;
	int32								EquipType;
	
	AbilitySystemComponent	= GetRaidBossAbilitySystemComponent();
	EquipmentItem			= GetItemInfo(EITemCategory::Equip, Index);
	ItemCDO					= Cast<URaidBossEquipmentItem>(EquipmentItem->GetItemCDO());
	EquipType				= ItemCDO ? ItemCDO->GetEquipType() : static_cast<int32>(EEquipType::None);
	
	AddToInventory = UnEquipItem(EquipType);
	
	if (AbilitySystemComponent && EquipmentItem && ItemCDO && InventoryWidget)
	{
		Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(EquipmentItem->ItemClass);
		SpecHandle = AbilitySystemComponent->GiveAbility(Spec);
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		
		EquippedItemInfo.Add(ItemCDO->GetEquipType(), EquipmentItem->ItemClass);
		DecreaseItemAmount(EITemCategory::Equip, Index);
		InventoryWidget->UpdateInventorySlot();
		InventoryWidget->UpdateEquipmentSlot();
	}
	
	AddNewItem(AddToInventory);
}

TSubclassOf<URaidBossItemBase> URaidBossInventorySystem::UnEquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	TSubclassOf<URaidBossItemBase>		ItemClass;
	FGameplayAbilitySpec*				Spec;
	
	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	ItemClass = EquippedItemInfo.FindRef(Index);
	
	if (AbilitySystemComponent && ItemClass && InventoryWidget)
	{
		Spec = AbilitySystemComponent->FindAbilitySpecFromClass(ItemClass);
		if (Spec)
		{
			AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
			AbilitySystemComponent->ClearAbility(Spec->Handle);
		}
		
		EquippedItemInfo.Remove(Index);
		InventoryWidget->UpdateInventorySlot();
		InventoryWidget->UpdateEquipmentSlot();
	}

	return ItemClass;
}

void URaidBossInventorySystem::UseConsumableItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	FItemInfomation*					ItemInfo;
	FGameplayAbilitySpec				Spec;
	FGameplayAbilitySpecHandle			SpecHandle;
	
	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	ItemInfo = GetItemInfo(EITemCategory::Consumable, Index);
	
	if (AbilitySystemComponent && ItemInfo && InventoryWidget)
	{
		Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(ItemInfo->ItemClass);
		SpecHandle = AbilitySystemComponent->GiveAbility(Spec);
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		AbilitySystemComponent->ClearAbility(SpecHandle);
		
		DecreaseItemAmount(EITemCategory::Consumable, Index);
		InventoryWidget->UpdateInventorySlot();
	}
}

void URaidBossInventorySystem::ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory)
{
	switch (ItemCategory)
	{
	case EITemCategory::Equip		: SwapItems(EquipItemInfo, Index1, Index2);		break;
	case EITemCategory::Consumable	: SwapItems(ConsumableItemInfo, Index1, Index2);	break;
	default: break;
	}

	if (InventoryWidget)
		InventoryWidget->UpdateInventorySlot();
}

void URaidBossInventorySystem::AddNewEquipItem(URaidBossItemBase* ItemCDO)
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

void URaidBossInventorySystem::AddNewConsumableItem(URaidBossItemBase* ItemCDO)
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

void URaidBossInventorySystem::SwapItems(TArray<FItemInfomation>& Items, int32 Index1, int32 Index2)
{
	FItemInfomation*	TmpItem;
	
	if (Items.IsValidIndex(Index1) && Items.IsValidIndex(Index2))
	{
		TmpItem = &Items[Index1];
		Items[Index1] = Items[Index2];
		Items[Index2] = *TmpItem;
	}
}

void URaidBossInventorySystem::IncreaseItemAmount(EITemCategory ItemCategory, int32 Index)
{
	bool				bIsValidItemIndex;

	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		EquipItemInfo[Index].Amount++;
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		ConsumableItemInfo[Index].Amount++;
	}
}

void URaidBossInventorySystem::DecreaseItemAmount(EITemCategory ItemCategory, int32 Index)
{
	bool	bIsValidItemIndex;

	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
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

void URaidBossInventorySystem::IncreaseCurrentItemCount(EITemCategory ItemCategory)
{
	switch (ItemCategory)
	{
		case EITemCategory::Consumable	: CurrentConsumableItemCount++; break;
		case EITemCategory::Equip		: CurrentEquipItemCount++;		break;
		default: break;
	}

	if (CurrentConsumableItemCount > MaximumItemAmount)
		CurrentConsumableItemCount = MaximumItemAmount;
	else if (CurrentEquipItemCount > MaximumItemAmount)
		CurrentEquipItemCount = MaximumItemAmount;
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
    	CurrentConsumableItemCount = 0;
    else if (CurrentEquipItemCount < 0)
    	CurrentEquipItemCount = 0;
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
		bIsValidItemIndex = false;

	return bIsValidItemIndex;
}

bool URaidBossInventorySystem::IsInventoryFull(EITemCategory ItemCategory)
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

ARaidBossPlayerControllerBase* URaidBossInventorySystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

const URaidBossEquipmentItem* URaidBossInventorySystem::GetEquipmentItem(int32 Index) const
{
	const URaidBossEquipmentItem*	EquipmentItem;
	FItemInfomation					CopyItemInfo;

	CopyItemInfo = EquipItemInfo[Index];
	EquipmentItem = Cast<const URaidBossEquipmentItem>(CopyItemInfo.GetItemCDO());

	return EquipmentItem;
}

const URaidBossItemBase* URaidBossInventorySystem::GetItemCDO(EITemCategory ItemCategory, int32 Index) const
{
	FItemInfomation	CopyItemInfo;
	bool			bIsValidItemIndex;

	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
		CopyItemInfo = EquipItemInfo[Index];
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
		CopyItemInfo = ConsumableItemInfo[Index];

	return CopyItemInfo.GetItemCDO();
}

URaidBossAbilitySystemComponent* URaidBossInventorySystem::GetRaidBossAbilitySystemComponent() const
{
	ARaidBossPlayerControllerBase*		Controller;
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;

	Controller = GetRaidBossPlayerControllerBase();
	AbilitySystemComponent = Controller ? Controller->GetRaidBossAbilitySystemComponent() : nullptr;

	return AbilitySystemComponent;
}

FItemInfomation* URaidBossInventorySystem::GetItemInfo(EITemCategory ItemCategory, int32 Index)
{
	FItemInfomation*	ItemInfo;
	bool				bIsValidItemIndex;

	ItemInfo = nullptr;
	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
		ItemInfo = &EquipItemInfo[Index];
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
		ItemInfo = &ConsumableItemInfo[Index];

	return ItemInfo;
}

const URaidBossEquipmentItem* URaidBossInventorySystem::GetEquippedItem(int32 Index) const
{
	const URaidBossEquipmentItem*	EquippedItem;
	TSubclassOf<URaidBossItemBase>	ItemClass;
	
	EquippedItem = nullptr;
	ItemClass = EquippedItemInfo.FindRef(Index);

	if (ItemClass)
		EquippedItem = Cast<const URaidBossEquipmentItem>(ItemClass->GetDefaultObject());
	
	return EquippedItem;
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
