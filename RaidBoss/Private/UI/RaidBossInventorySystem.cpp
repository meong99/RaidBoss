#include "UI/RaidBossInventorySystem.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/Widget/RaidBossInventoryWidget.h"

URaidBossInventorySystem::URaidBossInventorySystem(const FObjectInitializer& Initializer)
{
	EquipItemObjects.SetNum(MaximumItemCount);
	ConsumableItemObjects.SetNum(MaximumItemCount);
}

void URaidBossInventorySystem::AddNewItem(URaidBossItemBase* NewItem)
{
	if (NewItem->GetItemCategory() == EITemCategory::Equip && EquipItemObjects.Num() <= MaximumItemCount)
	{
		for (int32 i = 0; i < EquipItemObjects.Num(); i++)
		{
			if (EquipItemObjects[i] == nullptr)
			{
				EquipItemObjects[i] = Cast<URaidBossEquipmentItem>(NewItem);
				break;
			}
		}
	}
	else if (NewItem->GetItemCategory() == EITemCategory::Consumable && ConsumableItemObjects.Num() <= MaximumItemCount)
	{
		for (int32 i = 0; i < ConsumableItemObjects.Num(); i++)
		{
			if (ConsumableItemObjects[i] == nullptr)
			{
				ConsumableItemObjects[i] = Cast<URaidBossConsumableItem>(NewItem);
				break;
			}
		}
	}
	InventoryWidget->UpdateInventorySlot();
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

void URaidBossInventorySystem::ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory Category)
{
	switch (Category)
	{
		case EITemCategory::Equip		: SwapItems(EquipItemObjects, Index1, Index2);		break;
		case EITemCategory::Consumable	: SwapItems(ConsumableItemObjects, Index1, Index2);	break;
		default: break;
	}

	if (InventoryWidget)
		InventoryWidget->UpdateInventorySlot();
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

int32 URaidBossInventorySystem::GetMaximumItemCount() const
{
	return MaximumItemCount;
}

ARaidBossPlayerControllerBase* URaidBossInventorySystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

TArray<URaidBossItemBase*> URaidBossInventorySystem::GetEquipmentItems() const
{
	return EquipItemObjects;
}

TArray<URaidBossItemBase*> URaidBossInventorySystem::GetConsumableItemObjects() const
{
	return ConsumableItemObjects;
}

void URaidBossInventorySystem::SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget)
{
	InventoryWidget = InInventoryWidget;
	if (InventoryWidget)
	{
		InventoryWidget->SetWeakInventorySystem(this);
		InventoryWidget->CreateSlots();
	}
}
