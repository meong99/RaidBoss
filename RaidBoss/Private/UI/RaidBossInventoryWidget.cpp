#include "UI/RaidBossInventoryWidget.h"
#include "Components/Button.h"
#include "UI/RaidBossSlotWidget.h"
#include "Components/WrapBox.h"
#include "Management/RaidBossInventorySystem.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"

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

void URaidBossInventoryWidget::CreateSlot()
{
	CreateInventorySlot();
	InitEquipmentSlot();
}

void URaidBossInventoryWidget::CreateInventorySlot()
{
	int32 MaximumItemCount;
	
	if (WeakInventorySystem == nullptr || IsValid(InventorySlotClass) == false)
		return;

	MaximumItemCount = WeakInventorySystem->GetMaximumItemAmount();
	
	for (int i = 0; i < MaximumItemCount; i++)
	{
		URaidBossSlotWidget* ItemSlot = Cast<URaidBossSlotWidget>(CreateWidget(this, InventorySlotClass));
		if (ItemSlot)
		{
			ItemSlot->SetIndex(i);
			ItemSlot->SetVisibility(ESlateVisibility::Visible);
			ItemSlot->SetSlotType(ESlotType::ItemSlot);
			ItemSlot->SetWeakOwnerWidget(this);
			ItemSlot->SetPadding(FMargin(5.f));
			SlotWrapBox->AddChildToWrapBox(ItemSlot);
			InventorySlots.Add(ItemSlot);
		}
	}
}

void URaidBossInventoryWidget::InitEquipmentSlot()
{
	{
		HelmetSlot->SetIndex(static_cast<int32>(EEquipType::Helmet));
		HelmetSlot->SetVisibility(ESlateVisibility::Visible);
		HelmetSlot->SetSlotType(ESlotType::EquipmentSlot);
		HelmetSlot->SetWeakOwnerWidget(this);
	}
	{
		WeaponSlot->SetIndex(static_cast<int32>(EEquipType::Weapon));
		WeaponSlot->SetVisibility(ESlateVisibility::Visible);
		WeaponSlot->SetSlotType(ESlotType::EquipmentSlot);
		WeaponSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorTopSlot->SetIndex(static_cast<int32>(EEquipType::ArmorTop));
		ArmorTopSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorTopSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorTopSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorBottomSlot->SetIndex(static_cast<int32>(EEquipType::ArmorBottom));
		ArmorBottomSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorBottomSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorBottomSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorShoesSlot->SetIndex(static_cast<int32>(EEquipType::ArmorShoes));
		ArmorShoesSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorShoesSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorShoesSlot->SetWeakOwnerWidget(this);
	}
}

void URaidBossInventoryWidget::ResetInventorySlotImage()
{
	int32 Count;

	Count = InventorySlots.Num();
	
	for (int i = 0; i < Count; i++)
	{
		InventorySlots[i]->SetTexture(nullptr);
	}
}

void URaidBossInventoryWidget::EquipItem(int32 Index)
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->CallEquipItem(Index);
}

void URaidBossInventoryWidget::UnEquipItem(int32 Index)
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->CallUnEquipItem(Index);
}

void URaidBossInventoryWidget::UseItem(ESlotType SlotType, int32 Index)
{
	if (WeakInventorySystem == nullptr)
		return;
	
	switch (SlotType)
	{
		case ESlotType::EquipmentSlot : WeakInventorySystem->CallUnEquipItem(Index); break;
		case ESlotType::ItemSlot :
		{
			if (ShownInventory == EITemCategory::Equip)
				WeakInventorySystem->CallEquipItem(Index);
			else if (ShownInventory == EITemCategory::Consumable)
				WeakInventorySystem->UseConsumableItem(Index);
			break;
		}
		
		default: break;
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
	const URaidBossItemBase*	ItemCDO;
	int32						MaximumAmount;
	int32						ItemAmount;
	UTexture2D*					Texture;

	if (WeakInventorySystem == nullptr)
		return;

	ResetInventorySlotImage();

	MaximumAmount = WeakInventorySystem->GetMaximumItemAmount();
	for (int i = 0; i < MaximumAmount; i++)
	{
		ItemCDO = WeakInventorySystem->GetItemCDO(ShownInventory, i);
		ItemAmount = WeakInventorySystem->GetItemAmount(ShownInventory, i);
		
		if (ItemCDO != nullptr)
		{
			Texture = ItemCDO->GetItemTexture();
			InventorySlots[i]->SetTexture(Texture);
			InventorySlots[i]->SetItemAmount(FString::FromInt(ItemAmount));
		}
		else
			InventorySlots[i]->SetItemAmount("");
	}
}

void URaidBossInventoryWidget::UpdateEquipmentSlot()
{
	const URaidBossEquipmentItem*	Item;
	UTexture2D*						Texture2D;
	int32							EquipTypeSize;

	EquipTypeSize = static_cast<int32>(EEquipType::Size);
	
	for (int i = 0; i < EquipTypeSize; i++)
	{
		Item = WeakInventorySystem->GetEquippedItem(i);
		Texture2D = Item ? Item->GetItemTexture() : nullptr;
		
		switch (i)
		{
			case EEquipType::Helmet		 :	HelmetSlot->SetTexture(Texture2D);		break;
			case EEquipType::Weapon		 : 	WeaponSlot->SetTexture(Texture2D);		break;
			case EEquipType::ArmorTop	 : 	ArmorTopSlot->SetTexture(Texture2D);	break;
			case EEquipType::ArmorBottom : 	ArmorBottomSlot->SetTexture(Texture2D); break;
			case EEquipType::ArmorShoes	 : 	ArmorShoesSlot->SetTexture(Texture2D);	break;
			default: break;
		}
	}
}

void URaidBossInventoryWidget::ChangeItemOrder(int32 Index1, int32 Index2)
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->ChangeItemOrder(Index1, Index2, ShownInventory);
}

EITemCategory URaidBossInventoryWidget::GetShownInventory() const
{
	return ShownInventory;
}

const URaidBossEquipmentItem* URaidBossInventoryWidget::GetEquipmentItem(int32 Index) const
{
	if (WeakInventorySystem != nullptr)
		return WeakInventorySystem->GetEquipmentItem(Index);

	return nullptr;
}

void URaidBossInventoryWidget::SetWeakInventorySystem(URaidBossInventorySystem* InWeakInventorySystem)
{
	WeakInventorySystem = InWeakInventorySystem;
}

void URaidBossInventoryWidget::SetItemCategory(EITemCategory Category)
{
	ShownInventory = Category;
	ResetInventorySlotImage();
	UpdateInventorySlot();
}
