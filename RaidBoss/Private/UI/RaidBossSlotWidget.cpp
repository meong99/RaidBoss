#include "UI/RaidBossSlotWidget.h"
#include "UI/RaidBossInventoryWidget.h"
#include "UI/RaidBossQuickSlotWidget.h"
#include "UI/RaidBossSkillWidget.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void URaidBossSlotWidget::UseSlot() const
{
	URaidBossInventoryWidget*	InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	URaidBossSkillWidget*		SkillWidget = Cast<URaidBossSkillWidget>(WeakOwnerWidget);
	URaidBossQuickSlotWidget*	QuickWidget = Cast<URaidBossQuickSlotWidget>(WeakOwnerWidget);
	
	if (InventoryWidget)
		InventoryWidget->UseItem(SlotType, Index);

	if (SkillWidget)
		SkillWidget->UseSkill(Index);

	if (QuickWidget)
		QuickWidget->UseQuickSlot(Index);
}

void URaidBossSlotWidget::ResetThisSlot()
{
	ItemAmount = "";
	SetTexture(nullptr);
}

UTexture2D* URaidBossSlotWidget::GetTexture() const
{
	return UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(BindImage->Brush);
}

int32 URaidBossSlotWidget::GetItemAmount() const
{
	return FCString::Atoi(*ItemAmount);
}

void URaidBossSlotWidget::SetWeakOwnerWidget(UUserWidget* InWeakOwnerWidget)
{
	WeakOwnerWidget = InWeakOwnerWidget;
}

void URaidBossSlotWidget::SetTexture(UTexture2D* InTexture)
{
	if (InTexture)
		BindImage->SetBrushFromTexture(InTexture);
	else
		BindImage->SetBrushFromTexture(DefaultTexture);
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

void URaidBossSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* Textrue = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(BindImage->Brush);

	if (Textrue == nullptr && SlotType == ESlotType::EquipmentSlot)
		BindImage->SetBrushFromTexture(DefaultTexture);
}

bool URaidBossSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
	if (InOperation)
	{
		URaidBossSlotWidget* Payload = Cast<URaidBossSlotWidget>(InOperation->Payload);

		switch (SlotType)
		{
			case ESlotType::ItemSlot		: DropOnItemSlot(Payload);		break;
			case ESlotType::EquipmentSlot	: DropOnEquipmentSlot(Payload); break;
			case ESlotType::QuickSlot		: DropOnQuickSlot(Payload);		break;
			default: break;
		}
		
		return true;
	}
	
	return false;
}

void URaidBossSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	UTexture2D* Textrue = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(BindImage->Brush);
	bool		bIsValidSkillLevel = true;
	
	if (SlotType == ESlotType::SkillSlot)
	{
		URaidBossSkillWidget*	SkillWidget = Cast<URaidBossSkillWidget>(WeakOwnerWidget);

		if (SkillWidget)
		{
			if (SkillWidget->GetSkillLevel(Index) < 1)
			{
				bIsValidSkillLevel = false;
			}
		}
	}
	
	if (Textrue && bIsValidSkillLevel)
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
	
	UseSlot();
	
	FEventReply ReplyResult = UWidgetBlueprintLibrary::Handled();

	return ReplyResult.NativeReply; 
}

void URaidBossSlotWidget::DropOnItemSlot(const URaidBossSlotWidget* Payload) const
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

void URaidBossSlotWidget::DropOnEquipmentSlot(const URaidBossSlotWidget* Payload) const
{
	URaidBossInventoryWidget* InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
	if (Payload == nullptr || InventoryWidget == nullptr)
		return;
	
	if (IsEquippable(Payload))
	{
		InventoryWidget->EquipItem(Payload->Index);
	}
}

void URaidBossSlotWidget::DropOnQuickSlot(URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget*	PayloadInven = Cast<URaidBossInventoryWidget>(Payload->WeakOwnerWidget);
	URaidBossSkillWidget*		PayloadSkill = Cast<URaidBossSkillWidget>(Payload->WeakOwnerWidget);
	URaidBossQuickSlotWidget*	QuickSlotWidget = Cast<URaidBossQuickSlotWidget>(WeakOwnerWidget);
	
	if (QuickSlotWidget && PayloadInven && PayloadInven->GetShownInventory() == EITemCategory::Consumable)
	{
		QuickSlotWidget->RegisterItemKey(Index, ESlotType::ItemSlot, Payload->Index);
		QuickSlotWidget->SetInventoryWidget(PayloadInven);

		UTexture2D*	Texture = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(Payload->BindImage->Brush);
		SetTexture(Texture);
	}
	else if (QuickSlotWidget && PayloadSkill)
	{
		QuickSlotWidget->RegisterItemKey(Index, ESlotType::SkillSlot, Payload->Index);
		QuickSlotWidget->SetSkillWidget(PayloadSkill);
		SetItemAmount(0);
		
		UTexture2D*	Texture = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(Payload->BindImage->Brush);
		SetTexture(Texture);
	}
	else if (QuickSlotWidget)
	{
		QuickSlotWidget->MoveSlotData(Payload->Index, Index);
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
