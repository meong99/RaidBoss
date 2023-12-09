#include "UI/RaidBossSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void URaidBossSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* Textrue = UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(BindImage->Brush);

	if (Textrue == nullptr && SlotType == ESlotType::EquipmentSlot)
		BindImage->SetBrushFromTexture(DefaultTexture);
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
	
	FEventReply ReplyResult = UWidgetBlueprintLibrary::Handled();

	return ReplyResult.NativeReply; 
}

UTexture2D* URaidBossSlotWidget::GetTexture() const
{
	return UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(BindImage->Brush);
}

void URaidBossSlotWidget::SetTexture(UTexture2D* InTexture)
{
	if (InTexture)
		BindImage->SetBrushFromTexture(InTexture);
	else
		BindImage->SetBrushFromTexture(DefaultTexture);
}
