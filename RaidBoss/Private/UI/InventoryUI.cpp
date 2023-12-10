// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Components/WrapBox.h"
#include "UI/InventorySlot.h"

UInventoryUI::UInventoryUI()
{
}

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		CharacterBase->NotifyNewItemAdded.AddDynamic(this, &UInventoryUI::NotifyNewItemAddedCallBack);
	}
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
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
	
	Super::NativeDestruct();
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

void UInventoryUI::CreateItemSlots(TArray<UWidget*> InventoryWrapBox, ESlotType SlotType)
{
	for (int i = 0; i < SlotCount; i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		
		if (NewSlot)
		{
			NewSlot->SetSlotType(SlotType);
			InventoryWrapBox.Add(NewSlot);
		}
	}
}

void UInventoryUI::AddNewItemToSlot(TArray<UWidget*> InventoryWrapBox, URaidBossItemBase* NewItemCDO, int32 Amount) const
{
	if (InventoryWrapBox.IsEmpty() || NewItemCDO == nullptr || Amount <= 0)
	{
		return;
	}
	
	for (int  i = 0; i < InventoryWrapBox.Num(); i++)
	{
		UInventorySlot*	InventorySlot = Cast<UInventorySlot>(InventoryWrapBox[i]);

		if (InventorySlot == nullptr)
		{
			continue;
		}
		if (InventorySlot->GetItemAmount() > 0)
		{
			continue;
		}

		if (NewItemCDO->GetItemCategory() == EITemCategory::Equip)
		{
			InventorySlot->SetSlotType(ESlotType::EquipmentSlot);
		}
		else if (NewItemCDO->GetItemCategory() == EITemCategory::Consumable)
		{
			InventorySlot->SetSlotType(ESlotType::ConsumableSlot);
		}
		
		InventorySlot->RegisterNewItem(NewItemCDO, Amount);
		break;
	}
}
