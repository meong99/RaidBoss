// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Components/WrapBox.h"
#include "UI/InventorySlot.h"

void UInventoryUI::NativeOnInitialized()
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		CharacterBase->NotifyNewItemAdded.AddDynamic(this, &UInventoryUI::NotifyNewItemAddedCallBack);
	}
}

void UInventoryUI::NativeConstruct()
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());

	if (ControllerBase)
	{
		ControllerBase->SetInputMode(FInputModeGameAndUI{});
		ControllerBase->SetShowMouseCursor(true);
	}
}

void UInventoryUI::NativeDestruct()
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());

	if (ControllerBase)
	{
		ControllerBase->SetInputMode(FInputModeGameOnly{});
		ControllerBase->SetShowMouseCursor(false);
	}
}

void UInventoryUI::NotifyNewItemAddedCallBack(URaidBossItemBase* NewItemCDO, int32 Amount)
{
	if (NewItemCDO)
	{
		switch (NewItemCDO->GetItemCategory())
		{
		case EITemCategory::Equip :
			AddNewItemToSlot(EquipmentWrapBox->GetAllChildren(), NewItemCDO, Amount);
			break;
		case EITemCategory::Consumable :
			AddNewItemToSlot(ConsumableWrapBox->GetAllChildren(), NewItemCDO, Amount);
			break;
		default:
			break;
		}
	}
}

void UInventoryUI::AddNewItemToSlot(TArray<UWidget*> InventorySlots, URaidBossItemBase* NewItemCDO, int32 Amount) const
{
	if (InventorySlots.IsEmpty() || NewItemCDO == nullptr || Amount <= 0)
	{
		return;
	}
	
	for (int  i = 0; i < InventorySlots.Num(); i++)
	{
		UInventorySlot*	InventorySlot = Cast<UInventorySlot>(InventorySlots[i]);

		if (InventorySlot == nullptr)
		{
			continue;
		}
		if (InventorySlot->GetItemAmount() > 0)
		{
			continue;
		}
		
		InventorySlot->RegisterNewItem(NewItemCDO->GetAbilityTriggerTag(), Amount, NewItemCDO->GetItemTexture());
		break;
	}
}
