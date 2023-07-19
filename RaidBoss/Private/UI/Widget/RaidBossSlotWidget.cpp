#include "UI/Widget/RaidBossSlotWidget.h"
#include "UI/Widget/RaidBossInventoryWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

bool URaidBossSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
	URaidBossSlotWidget*	Payload;
	
	if (InOperation)
	{
		Payload = Cast<URaidBossSlotWidget>(InOperation->Payload);

		switch (SlotType)
		{
			case ESlotType::ItemSlot : DropOnItemSlot(Payload); break;
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
		OutOperation->Pivot = EDragPivot::MouseDown;
	}
}

FReply URaidBossSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply ReplyResult;
	
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	
	return ReplyResult.NativeReply;
}

void URaidBossSlotWidget::DropOnItemSlot(URaidBossSlotWidget* Payload)
{
	URaidBossInventoryWidget*	InventoryWidget = Cast<URaidBossInventoryWidget>(WeakOwnerWidget);
	
	if (Payload == nullptr || InventoryWidget == nullptr)
		return;

	switch (Payload->SlotType)
	{
		case ESlotType::ItemSlot : InventoryWidget->ChangeItemOrder(Index, Payload->Index); break;
		default: break;
	}
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

void URaidBossSlotWidget::SetSlotType(ESlotType InSlotType)
{
	SlotType = InSlotType;
}
