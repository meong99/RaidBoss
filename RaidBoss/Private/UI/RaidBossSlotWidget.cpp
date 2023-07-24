﻿#include "UI/RaidBossSlotWidget.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "UI/RaidBossInventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

bool URaidBossSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
	const URaidBossSlotWidget*	Payload;
	
	if (InOperation)
	{
		Payload = Cast<URaidBossSlotWidget>(InOperation->Payload);

		switch (SlotType)
		{
			case ESlotType::ItemSlot		: DropOnItemSlot(Payload);	break;
			case ESlotType::EquipmentSlot	: DropOnEquipmentSlot(Payload); break;
			default: break;
		}
		
		return true;
	}
	
	return false;
}

void URaidBossSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (Texture)
	{
		BindImage->SetDesiredSizeOverride(FVector2d(50, 50));
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
		OutOperation->Payload = this;
		OutOperation->DefaultDragVisual = BindImage;
		OutOperation->Pivot = DragPivot;
		OutOperation->Offset = DragOffset;
	}
}

FReply URaidBossSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply ReplyResult;
	
	ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	
	return ReplyResult.NativeReply;
}

FReply URaidBossSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	
	URaidBossInventoryWidget*	InventoryWidget;
	FEventReply					ReplyResult;
	bool						bIsVisible;
	
	InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	bIsVisible		= (BindImage->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
	
	if (bIsVisible && InventoryWidget)
	{
		InventoryWidget->UseItem(SlotType, Index);
		ReplyResult = UWidgetBlueprintLibrary::Handled();
	}
	
	return ReplyResult.NativeReply; 
}

void URaidBossSlotWidget::DropOnItemSlot(const URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget*	InventoryWidget;

	InventoryWidget= Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
	if (Payload == nullptr || InventoryWidget == nullptr)
		return;

	switch (Payload->SlotType)
	{
		case ESlotType::ItemSlot		: InventoryWidget->ChangeItemOrder(Index, Payload->Index); break;
		case ESlotType::EquipmentSlot	: InventoryWidget->UnEquipItem(Payload->Index); break;
		default: break;
	}
}

void URaidBossSlotWidget::DropOnEquipmentSlot(const URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget*	InventoryWidget;
	
	InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
	if (Payload == nullptr || InventoryWidget == nullptr)
		return;
	
	if (IsEquippable(Payload))
	{
		InventoryWidget->EquipItem(Payload->Index);
	}
}

bool URaidBossSlotWidget::IsEquippable(const URaidBossSlotWidget* Payload) const
{
	const URaidBossInventoryWidget*	InventoryWidget;
	const URaidBossEquipmentItem*	Item;
	bool							bIsEquipmentWindow;
	bool							bIsFromItemSlot;
	bool							bIsEquippable;

	InventoryWidget		= Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	bIsEquipmentWindow	= InventoryWidget->GetShownInventory() == EITemCategory::Equip;
	bIsFromItemSlot		= Payload->SlotType == ESlotType::ItemSlot;
	bIsEquippable		= false;
	
	if (InventoryWidget)
	{
		Item = InventoryWidget->GetEquipmentItem(Payload->Index);

		if (Item && Item->GetEquipType() == Index)
			bIsEquippable = true;
	}

	return bIsEquipmentWindow && bIsFromItemSlot && bIsEquippable;
}

void URaidBossSlotWidget::SetWeakOwnerWidget(UUserWidget* InWeakOwnerWidget)
{
	WeakOwnerWidget = InWeakOwnerWidget;
}

void URaidBossSlotWidget::SetTexture(UTexture2D* InTexture)
{
	Texture = InTexture;
	if (Texture)
		BindImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		BindImage->SetVisibility(ESlateVisibility::Hidden);
}

void URaidBossSlotWidget::SetIndex(int32 InIndex)
{
	Index = InIndex;
}

void URaidBossSlotWidget::SetItemAmount(FString Amount)
{
	ItemAmount = Amount;
}

void URaidBossSlotWidget::SetSlotType(ESlotType InSlotType)
{
	SlotType = InSlotType;
}
