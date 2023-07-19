#include "UI/Widget/RaidBossInventoryWidget.h"
#include "Components/Button.h"
#include "UI/Widget/RaidBossSlotWidget.h"
#include "Components/WrapBox.h"
#include "UI/RaidBossInventorySystem.h"

URaidBossInventoryWidget::URaidBossInventoryWidget(const FObjectInitializer& Initializer) : Super(Initializer)
{
}

bool URaidBossInventoryWidget::Initialize()
{
	if (Super::Initialize())
	{
		EquipButton->OnClicked.AddDynamic(this, &URaidBossInventoryWidget::ClickedEquipButton);
		ConsumableButton->OnClicked.AddDynamic(this, &URaidBossInventoryWidget::ClickedConsumableButton);
		MiscellaneousButton->OnClicked.AddDynamic(this, &URaidBossInventoryWidget::ClickedMiscellaneousButton);
		
		return true;
	}
	
	return false;
}

void URaidBossInventoryWidget::CreateSlots()
{
	int32 MaximumItemCount;
	
	if (WeakInventorySystem == nullptr || IsValid(SlotClass) == false)
		return;

	MaximumItemCount = WeakInventorySystem->GetMaximumItemCount();
	
	for (int i = 0; i < MaximumItemCount; i++)
	{
		URaidBossSlotWidget* ItemSlot = Cast<URaidBossSlotWidget>(CreateWidget(this, SlotClass, FName(FString::FromInt(i))));
		if (ItemSlot)
		{
			ItemSlot->SetIndex(i);
			ItemSlot->SetVisibility(ESlateVisibility::Visible);
			ItemSlot->SetSlotType(ESlotType::ItemSlot);
			ItemSlot->SetWeakOwnerWidget(this);
			ItemSlot->SetPadding(FMargin(5.f));
			SlotWrapBox->AddChildToWrapBox(ItemSlot);
			ItemSlots.Add(ItemSlot);
		}
	}
}

void URaidBossInventoryWidget::ResetSlotImage()
{
	int32 Count;

	Count = ItemSlots.Num();
	
	for (int i = 0; i < Count; i++)
	{
		ItemSlots[i]->SetTexture(nullptr);
	}
}

void URaidBossInventoryWidget::ClickedEquipButton()
{
	SetItemCategory(EITemCategory::Equip);
}

void URaidBossInventoryWidget::ClickedConsumableButton()
{
	SetItemCategory(EITemCategory::Consumable);
}

void URaidBossInventoryWidget::ClickedMiscellaneousButton()
{
	SetItemCategory(EITemCategory::Miscellaneous);
}

void URaidBossInventoryWidget::UpdateInventorySlot()
{
	TArray<URaidBossItemBase*>	Items;
	int32						Count;
	UTexture2D*					Texture;

	if (WeakInventorySystem == nullptr)
		return;

	ResetSlotImage();
	
	switch (ItemCategory)
	{
		case EITemCategory::Equip		: Items = WeakInventorySystem->GetEquipmentItems();			break;
		case EITemCategory::Consumable	: Items = WeakInventorySystem->GetConsumableItemObjects();	break;
		default: return;
	}

	Count = WeakInventorySystem->GetMaximumItemCount();
	for (int i = 0; i < Count; i++)
	{
		if (Items[i] != nullptr)
		{
			Texture = Items[i]->GetItemTexture();
			ItemSlots[i]->SetTexture(Texture);
		}
	}
}

void URaidBossInventoryWidget::ChangeItemOrder(int32 Index1, int32 Index2)
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->ChangeItemOrder(Index1, Index2, ItemCategory);
}

void URaidBossInventoryWidget::SetWeakInventorySystem(URaidBossInventorySystem* InWeakInventorySystem)
{
	WeakInventorySystem = InWeakInventorySystem;
}

void URaidBossInventoryWidget::SetItemCategory(EITemCategory Category)
{
	ItemCategory = Category;
	ResetSlotImage();
	UpdateInventorySlot();
}
