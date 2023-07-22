#include "Management/RaidBossInventorySystem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/RaidBossEquipmentWidget.h"
#include "UI/RaidBossInventoryWidget.h"

URaidBossInventorySystem::URaidBossInventorySystem(const FObjectInitializer& Initializer)
{
	EquipItemInfo.SetNum(MaximumItemCount);
	ConsumableItemInfo.SetNum(MaximumItemCount);
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
//
// void URaidBossInventorySystem::AddNewItem(URaidBossItemBase* NewItem)
// {
// 	if (NewItem && NewItem->GetItemCategory() == EITemCategory::Equip && EquipItemObjects.Num() <= MaximumItemCount)
// 	{
// 		for (int32 i = 0; i < EquipItemObjects.Num(); i++)
// 		{
// 			if (EquipItemObjects[i] == nullptr)
// 			{
// 				EquipItemObjects[i] = Cast<URaidBossEquipmentItem>(NewItem);
// 				break;
// 			}
// 		}
// 	}
// 	else if (NewItem && NewItem->GetItemCategory() == EITemCategory::Consumable && ConsumableItemObjects.Num() <= MaximumItemCount)
// 	{
// 		for (int32 i = 0; i < ConsumableItemObjects.Num(); i++)
// 		{
// 			if (ConsumableItemObjects[i] == nullptr)
// 			{
// 				ConsumableItemObjects[i] = Cast<URaidBossConsumableItem>(NewItem);
// 				break;
// 			}
// 		}
// 	}
// 	InventoryWidget->UpdateInventorySlot();
// }

void URaidBossInventorySystem::AddNewItem(const TSubclassOf<URaidBossItemBase>& NewItem)
{
	URaidBossItemBase* ItemCDO;
	
	// ItemCDO = NewItem ? Cast<URaidBossItemBase>(NewItem->GetDefaultObject()) : nullptr;
	ItemCDO = Cast<URaidBossItemBase>(NewItem->GetDefaultObject());
	
	if (ItemCDO == nullptr || IsInventoryFull(ItemCDO->GetItemCategory()))
		return;
	
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
		case EITemCategory::Equip		: EquipItemInfo[Index].Reset;			break;
		case EITemCategory::Consumable	: ConsumableItemInfo[Index].Reset();	break;
		default: break;
	}
}

void URaidBossInventorySystem::CallEquipItem(int32 Index)
{
	EquipItem(Index);
}

void URaidBossInventorySystem::CallUnEquipItem(int32 Index)
{
	URaidBossEquipmentItem*	UnEquippedItem;
	
	UnEquippedItem = UnEquipItem(Index);

	AddNewItem(UnEquippedItem);
}

void URaidBossInventorySystem::EquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	URaidBossEquipmentItem*				EquipmentItem;
	URaidBossEquipmentItem*				AddToInventory;
	FGameplayAbilitySpec				Spec;
	FGameplayAbilitySpecHandle			SpecHandle;
	
	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	EquipmentItem = Cast<URaidBossEquipmentItem>(GetItem(EITemCategory::Equip, Index));
	
	AddToInventory = UnEquipItem(EquipmentItem->GetEquipType());
	
	if (AbilitySystemComponent && EquipmentItem && InventoryWidget)
	{
		Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(EquipmentItem->GetClass());
		SpecHandle = AbilitySystemComponent->GiveAbility(Spec);
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		
		RemoveItem(EITemCategory::Equip, Index);
		EquippedItems.Add(EquipmentItem->GetEquipType(), EquipmentItem);
		InventoryWidget->UpdateInventorySlot();
		InventoryWidget->UpdateEquipmentSlot();
	}

	AddNewItem(AddToInventory);
}

URaidBossEquipmentItem* URaidBossInventorySystem::UnEquipItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	URaidBossEquipmentItem*				EquipmentItem;
	FGameplayAbilitySpec*				Spec;
	
	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	EquipmentItem = EquippedItems.FindRef(Index);
	
	if (AbilitySystemComponent && EquipmentItem && InventoryWidget)
	{
		Spec = AbilitySystemComponent->FindAbilitySpecFromClass(EquipmentItem->GetClass());
		if (Spec)
		{
			AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
			AbilitySystemComponent->ClearAbility(Spec->Handle);
		}
		
		EquippedItems.Remove(Index);
		InventoryWidget->UpdateInventorySlot();
		InventoryWidget->UpdateEquipmentSlot();
	}

	return EquipmentItem;
}

void URaidBossInventorySystem::UseConsumableItem(int32 Index)
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	URaidBossConsumableItem*			ConsumableItem;
	FGameplayAbilitySpec				Spec;
	FGameplayAbilitySpecHandle			SpecHandle;
	
	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	ConsumableItem = Cast<URaidBossConsumableItem>(GetItem(EITemCategory::Consumable, Index));
	
	if (AbilitySystemComponent && ConsumableItem && InventoryWidget)
	{
		Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(ConsumableItem->GetClass());
		SpecHandle = AbilitySystemComponent->GiveAbility(Spec);
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		AbilitySystemComponent->ClearAbility(SpecHandle);
		
		DecreaseItemCount(EITemCategory::Consumable, Index);
		InventoryWidget->UpdateInventorySlot();
	}
}

void URaidBossInventorySystem::ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory ItemCategory)
{
	switch (ItemCategory)
	{
	case EITemCategory::Equip		: SwapItems(EquipItemObjects, Index1, Index2);		break;
	case EITemCategory::Consumable	: SwapItems(ConsumableItemObjects, Index1, Index2);	break;
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
		if (EquipItemInfo[i].IsEmpty())
		{
			EmptyIndex = i;
		}
		else if (EquipItemInfo[i].ItemClass == ItemCDO->GetClass())
		{
			EquipItemInfo[i].ItemClass = ItemCDO->GetClass();
			EquipItemInfo[i].Amount++;
			
			return;
		}
	}
	if (EmptyIndex != INDEX_NONE)
	{
		EquipItemInfo[EmptyIndex].ItemClass = ItemCDO->GetClass();
		EquipItemInfo[EmptyIndex].Amount++;
	}
}

void URaidBossInventorySystem::AddNewConsumableItem(URaidBossItemBase* ItemCDO)
{
	int32 EmptyIndex = INDEX_NONE;
	
	for (int32 i = 0; i < ConsumableItemInfo.Num(); i++)
	{
		if (ConsumableItemInfo[i].IsEmpty())
		{
			EmptyIndex = i;
		}
		else if (ConsumableItemInfo[i].ItemClass == ItemCDO->GetClass())
		{
			ConsumableItemInfo[i].ItemClass = ItemCDO->GetClass();
			ConsumableItemInfo[i].Amount++;
			
			return;
		}
	}
	if (EmptyIndex != INDEX_NONE)
	{
		ConsumableItemInfo[EmptyIndex].ItemClass = ItemCDO->GetClass();
		ConsumableItemInfo[EmptyIndex].Amount++;
	}
}

void URaidBossInventorySystem::SwapItems(TArray<URaidBossItemBase*>& Items, int32 Index1, int32 Index2)
{
	URaidBossItemBase*	TmpItem;
	
	if (Items.IsValidIndex(Index1) && Items.IsValidIndex(Index2))
	{
		TmpItem = Items[Index1];
		Items[Index1] = Items[Index2];
		Items[Index2] = TmpItem;
	}
}

void URaidBossInventorySystem::IncreaseItemCount(EITemCategory ItemCategory, int32 Index)
{
	URaidBossItemBase*	Item;
	int32				Amount;
	bool				bIsValidItemIndex;
	bool				DoCheckObject;

	DoCheckObject = true;
	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index, DoCheckObject);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		Item = EquipItemObjects[Index];
		Amount = Item->GetItemAmount();

		Item->SetItemAmount(Amount + 1);
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		Item = ConsumableItemObjects[Index];
		Amount = Item->GetItemAmount();

		Item->SetItemAmount(Amount + 1);
	}
}

void URaidBossInventorySystem::DecreaseItemCount(EITemCategory ItemCategory, int32 Index)
{
	URaidBossItemBase*	Item;
	int32				Amount;
	bool				bIsValidItemIndex;
	bool				DoCheckObject;

	DoCheckObject = true;
	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index, DoCheckObject);
	
	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
	{
		Item = EquipItemObjects[Index];
		Amount = Item->GetItemAmount();

		Amount > 1 ? Item->SetItemAmount(Amount - 1) : RemoveItem(ItemCategory, Index);
	}
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
	{
		Item = ConsumableItemObjects[Index];
		Amount = Item->GetItemAmount();

		Amount > 1 ? Item->SetItemAmount(Amount - 1) : RemoveItem(ItemCategory, Index);
	}
}

bool URaidBossInventorySystem::IsValidItemIndex(EITemCategory ItemCategory, int32 Index, bool DoCheckObject)
{
	bool bIsValidItemIndex;

	if (ItemCategory == EITemCategory::Equip)
		bIsValidItemIndex = (EquipItemObjects.IsValidIndex(Index) && EquipItemObjects[Index] != nullptr);
	else if (ItemCategory == EITemCategory::Consumable)
		bIsValidItemIndex = (ConsumableItemObjects.IsValidIndex(Index) && ConsumableItemObjects[Index] != nullptr);
	else
		bIsValidItemIndex = false;

	return bIsValidItemIndex;
}

bool URaidBossInventorySystem::IsInventoryFull(EITemCategory ItemCategory)
{
	bool	bIsInventoryFull;
	
	switch (ItemCategory)
	{
		case EITemCategory::Equip		: bIsInventoryFull = (EquipItemInfo.Num() >= MaximumItemCount);		break;
		case EITemCategory::Consumable	: bIsInventoryFull = (ConsumableItemInfo.Num() >= MaximumItemCount);	break;
			
		default : bIsInventoryFull = true; break;
	}

	return bIsInventoryFull;
}

int32 URaidBossInventorySystem::GetMaximumItemCount() const
{
	return MaximumItemCount;
}

ARaidBossPlayerControllerBase* URaidBossInventorySystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

URaidBossAbilitySystemComponent* URaidBossInventorySystem::GetRaidBossAbilitySystemComponent() const
{
	ARaidBossPlayerControllerBase*		Controller;
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;

	Controller = GetRaidBossPlayerControllerBase();
	AbilitySystemComponent = Controller ? Controller->GetRaidBossAbilitySystemComponent() : nullptr;

	return AbilitySystemComponent;
}

URaidBossItemBase* URaidBossInventorySystem::GetItem(EITemCategory ItemCategory, int32 Index)
{
	URaidBossItemBase*	ItemObject;
	bool				bIsValidItemIndex;
	bool				DoCheckObject;

	DoCheckObject = true;
	bIsValidItemIndex = IsValidItemIndex(ItemCategory, Index, DoCheckObject);

	if (ItemCategory == EITemCategory::Equip && bIsValidItemIndex)
		ItemObject = EquipItemObjects[Index];
	else if (ItemCategory == EITemCategory::Consumable && bIsValidItemIndex)
		ItemObject = ConsumableItemObjects[Index];
	else
		ItemObject = nullptr;

	return ItemObject;
}

const URaidBossEquipmentItem* URaidBossInventorySystem::GetEquippedItem(int32 Index)
{
	return EquippedItems.FindRef(Index);
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
