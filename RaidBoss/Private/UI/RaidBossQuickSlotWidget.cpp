#include "UI/RaidBossQuickSlotWidget.h"
#include "UI/RaidBossInventoryWidget.h"
#include "UI/RaidBossSkillWidget.h"
#include "UI/RaidBossSlotWidget.h"

void URaidBossQuickSlotWidget::UseQuickSlot(int32 Index)
{
	FQuickSlotData* SlotData = RegisteredDataMap.Find(Index);

	if (SlotData)
	{
		if (SlotData->SlotType == ESlotType::ItemSlot && InventoryWidget)
		{
			InventoryWidget->UseConsumableItem(SlotData->AbilityIndex);
		}
		else if (SlotData->SlotType == ESlotType::SkillSlot && SkillWidget)
		{
			SkillWidget->UseSkill(SlotData->AbilityIndex);
		}
	}
}

void URaidBossQuickSlotWidget::ResetQuickSlot(int32 Index)
{
	if (SlotWidgets.IsValidIndex(Index))
	{
		SlotWidgets[Index]->SetTexture(nullptr);
		SlotWidgets[Index]->SetItemAmount(0);
	}
	
	RegisteredDataMap.Remove(Index);
}

void URaidBossQuickSlotWidget::RegisterItemKey(int32 SlotIndex, ESlotType SlotType, int32 AbilityIndex)
{
	if (SlotType != ESlotType::ItemSlot && SlotType != ESlotType::SkillSlot)
		return;
	
	int32 ValueToChange = INDEX_NONE;
	
	for (const auto Element : RegisteredDataMap)
	{
		if (Element.Value.SlotType == SlotType && Element.Value.AbilityIndex == AbilityIndex)
		{
			ValueToChange = Element.Key;
		}
	}
	
	if (ValueToChange != INDEX_NONE)
	{
		MoveSlotData(ValueToChange, SlotIndex);
	}
	else
	{
		RegisteredDataMap.Add(SlotIndex, FQuickSlotData(SlotType, AbilityIndex));
	}
}

void URaidBossQuickSlotWidget::MoveSlotData(int32 Source, int32 Destination)
{
	if (Source == Destination)
	{
		return;
	}
	
	if (SlotWidgets.IsValidIndex(Destination) && SlotWidgets.IsValidIndex(Source))
	{
		SlotWidgets[Destination]->SetTexture(SlotWidgets[Source]->GetTexture());
		SlotWidgets[Destination]->SetItemAmount(SlotWidgets[Source]->GetItemAmount());
	
		SlotWidgets[Source]->ResetThisSlot();

		FQuickSlotData* SlotData = RegisteredDataMap.Find(Source);
		
		if (SlotData)
		{
			RegisteredDataMap.Add(Destination, *SlotData);
		}
		
		RegisteredDataMap.Remove(Source);
	}
}

ESlotType URaidBossQuickSlotWidget::GetRegisteredSlotType(int32 Index) const
{
	const FQuickSlotData* SlotData = RegisteredDataMap.Find(Index);
	
	if (SlotData)
	{
		return SlotData->SlotType;
	}

	return ESlotType::None;
}

void URaidBossQuickSlotWidget::SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget)
{
	InventoryWidget = InInventoryWidget;
}

void URaidBossQuickSlotWidget::SetSkillWidget(URaidBossSkillWidget* InSkillWidget)
{
	SkillWidget = InSkillWidget;
}

void URaidBossQuickSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (int i = 0; i < SlotCount; i++)
	{
		URaidBossSlotWidget* QuickSlot = Cast<URaidBossSlotWidget>(CreateWidget(this, SlotClass));
		
		if (QuickSlot)
		{
			QuickSlot->SetSlotType(ESlotType::QuickSlot);
			QuickSlot->SetIndex(i);
			QuickSlot->SetVisibility(ESlateVisibility::Visible);
			QuickSlot->SetWeakOwnerWidget(this);
			QuickSlot->SetPadding(FMargin(10.f));
			QuickSlotBox->AddChildToHorizontalBox(QuickSlot);
			SlotWidgets.Add(QuickSlot);
		}
	}
}

void URaidBossQuickSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int i = 0; i < SlotWidgets.Num(); i++)
	{
		FQuickSlotData*	SlotData = RegisteredDataMap.Find(i);
		
		if (InventoryWidget && SlotData && SlotData->SlotType == ESlotType::ItemSlot)
		{
			int32 ItemAmount = InventoryWidget->GetConsumableItemAmount(SlotData->AbilityIndex);
			
			SlotWidgets[i]->SetItemAmount(ItemAmount);
			
			if (ItemAmount == 0)
			{
				SlotWidgets[i]->ResetThisSlot();
				RegisteredDataMap.Remove(i);
			}
		}
	}
}
