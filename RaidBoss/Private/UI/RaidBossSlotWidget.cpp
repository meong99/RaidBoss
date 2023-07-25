﻿#include "UI/RaidBossSlotWidget.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "UI/RaidBossInventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "UI/RaidBossSkillWidget.h"

void URaidBossSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (RefPayload)
	{
		ItemAmount = RefPayload->ItemAmount;
		
		if (ItemAmount == "")
		{
			RefPayload = nullptr;
			SetTexture(nullptr);
		}
	}
}

void URaidBossSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotType == ESlotType::ItemSlot || SlotType == ESlotType::EquipmentSlot)
		BindImage->SetVisibility(ESlateVisibility::Hidden);
	else if (SlotType == ESlotType::QuickSlot)
		UpdateCanTick();
}

bool URaidBossSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
	if (InOperation)
	{
		const URaidBossSlotWidget* Payload = Cast<URaidBossSlotWidget>(InOperation->Payload);

		switch (SlotType)
		{
			case ESlotType::ItemSlot		: DropOnItemSlot(Payload);	break;
			case ESlotType::EquipmentSlot	: DropOnEquipmentSlot(Payload); break;
			case ESlotType::QuickSlot		: DropOnQuickSlot(Payload);
			default: break;
		}
		
		return true;
	}
	
	return false;
}

void URaidBossSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (BindImage->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
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

	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	
	return ReplyResult.NativeReply;
}

FReply URaidBossSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	
	FEventReply	ReplyResult;

	if (SlotType == ESlotType::ItemSlot || SlotType == ESlotType::EquipmentSlot)
	{
		URaidBossInventoryWidget*	InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
		bool						bIsVisible = (BindImage->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
		
		if (bIsVisible && InventoryWidget)
		{
			InventoryWidget->UseItem(SlotType, Index);
		}
	}
	else if (SlotType == ESlotType::SkillSlot)
	{
		URaidBossSkillWidget* SkillWidget = Cast<URaidBossSkillWidget>(WeakOwnerWidget);

		if (SkillWidget)
			SkillWidget->UseSkill(Index);
	}
	else if (SlotType == ESlotType::QuickSlot)
	{
		URaidBossInventoryWidget* InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);

		if (InventoryWidget)
		{
			InventoryWidget->UseItem(ESlotType::ItemSlot, Index);
		}
		
		URaidBossSkillWidget* SkillWidget = Cast<URaidBossSkillWidget>(WeakOwnerWidget);

		if (SkillWidget)
		{
			SkillWidget->UseSkill(Index);
		}
	}
	
	ReplyResult = UWidgetBlueprintLibrary::Handled();

	return ReplyResult.NativeReply; 
}

void URaidBossSlotWidget::DropOnItemSlot(const URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget* InventoryWidget= Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
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
	URaidBossInventoryWidget* InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
	if (Payload == nullptr || InventoryWidget == nullptr)
		return;
	
	if (IsEquippable(Payload))
	{
		InventoryWidget->EquipItem(Payload->Index);
	}
}

void URaidBossSlotWidget::DropOnQuickSlot(const URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget*	PayloadInven = Cast<URaidBossInventoryWidget>(Payload->WeakOwnerWidget);
	URaidBossSkillWidget*		PayloadSkill = Cast<URaidBossSkillWidget>(Payload->WeakOwnerWidget);
	
	if (PayloadInven && PayloadInven->GetShownInventory() == EITemCategory::Consumable)
	{
		WeakOwnerWidget = PayloadInven;
		RefPayload = Payload;
		Index = Payload->Index;
		ItemAmount = Payload->ItemAmount;
		
		UTexture2D*	Texture = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(Payload->BindImage->Brush);
		SetTexture(Texture);
	}
	else if (PayloadSkill)
	{
		WeakOwnerWidget = PayloadSkill;
		RefPayload = nullptr;
		Index = Payload->Index;
		SetItemAmount(0);

		UTexture2D*	Texture = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(Payload->BindImage->Brush);
		SetTexture(Texture);
	}
}

bool URaidBossSlotWidget::IsEquippable(const URaidBossSlotWidget* Payload) const
{
	const URaidBossInventoryWidget* InventoryWidget	= Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	bool							bIsEquippable	= false;
	
	if (InventoryWidget )
	{
		const URaidBossEquipmentItem* Item = InventoryWidget->GetEquipmentItem(Payload->Index);

		if (Item && Item->GetEquipType() == Index)
			bIsEquippable = true;
	}
	
	bool	bIsEquipmentWindow	= (InventoryWidget->GetShownInventory() == EITemCategory::Equip);
	bool	bIsFromItemSlot		= (Payload->SlotType == ESlotType::ItemSlot);

	return (bIsEquipmentWindow && bIsFromItemSlot && bIsEquippable);
}

void URaidBossSlotWidget::SetWeakOwnerWidget(UUserWidget* InWeakOwnerWidget)
{
	WeakOwnerWidget = InWeakOwnerWidget;
}

void URaidBossSlotWidget::SetTexture(UTexture2D* InTexture)
{
	BindImage->SetBrushFromTexture(InTexture);
	
	if (InTexture)
		BindImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		BindImage->SetVisibility(ESlateVisibility::Hidden);
}

void URaidBossSlotWidget::SetIndex(int32 InIndex)
{
	Index = InIndex;
}

void URaidBossSlotWidget::SetItemAmount(int32 Amount)
{
	if (Amount > 0)
		ItemAmount = FString::FromInt(Amount);
	else
		ItemAmount = "";
}

void URaidBossSlotWidget::SetSlotType(ESlotType InSlotType)
{
	SlotType = InSlotType;
}
