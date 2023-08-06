#include "UI/RaidBossInventoryWidget.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"

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
	if (WeakInventorySystem == nullptr || IsValid(InventorySlotClass) == false)
		return;

	int32 MaximumItemCount = WeakInventorySystem->GetMaximumItemAmount();
	
	for (int i = 0; i < MaximumItemCount; i++)
	{
		URaidBossSlotWidget* ItemSlot = Cast<URaidBossSlotWidget>(CreateWidget(this, InventorySlotClass));
		
		if (ItemSlot)
		{
			ItemSlot->SetSlotType(ESlotType::ItemSlot);
			ItemSlot->SetIndex(i);
			ItemSlot->SetVisibility(ESlateVisibility::Visible);
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
		HelmetSlot->SetSlotType(ESlotType::EquipmentSlot);
		HelmetSlot->SetIndex(static_cast<int32>(EEquipType::Helmet));
		HelmetSlot->SetVisibility(ESlateVisibility::Visible);
		HelmetSlot->SetWeakOwnerWidget(this);
	}
	{
		WeaponSlot->SetSlotType(ESlotType::EquipmentSlot);
		WeaponSlot->SetIndex(static_cast<int32>(EEquipType::Weapon));
		WeaponSlot->SetVisibility(ESlateVisibility::Visible);
		WeaponSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorTopSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorTopSlot->SetIndex(static_cast<int32>(EEquipType::ArmorTop));
		ArmorTopSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorTopSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorBottomSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorBottomSlot->SetIndex(static_cast<int32>(EEquipType::ArmorBottom));
		ArmorBottomSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorBottomSlot->SetWeakOwnerWidget(this);
	}
	{
		ArmorShoesSlot->SetSlotType(ESlotType::EquipmentSlot);
		ArmorShoesSlot->SetIndex(static_cast<int32>(EEquipType::ArmorShoes));
		ArmorShoesSlot->SetVisibility(ESlateVisibility::Visible);
		ArmorShoesSlot->SetWeakOwnerWidget(this);
	}
}

void URaidBossInventoryWidget::EquipItem(int32 Index) const
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->CallEquipItem(Index);
}

void URaidBossInventoryWidget::UnEquipItem(int32 Index) const
{
	if (WeakInventorySystem != nullptr)
		WeakInventorySystem->CallUnEquipItem(Index);
}

void URaidBossInventoryWidget::UseConsumableItem(int32 Index) const
{
	WeakInventorySystem->UseConsumableItem(Index);
}

void URaidBossInventoryWidget::UseItem(ESlotType SlotType, int32 Index) const
{
	if (WeakInventorySystem == nullptr)
		return;
	
	switch (SlotType)
	{
		case ESlotType::EquipmentSlot : WeakInventorySystem->CallUnEquipItem(Index); break;
		case ESlotType::ItemSlot :
		{
			if (ShownInventory == EITemCategory::Equip)
			{
				WeakInventorySystem->CallEquipItem(Index);
			}
			else if (ShownInventory == EITemCategory::Consumable)
			{
				WeakInventorySystem->UseConsumableItem(Index);
			}
				
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

void URaidBossInventoryWidget::ChangeItemOrder(int32 Index1, int32 Index2) const
{
	if (WeakInventorySystem != nullptr)
	{
		WeakInventorySystem->ChangeItemOrder(Index1, Index2, ShownInventory);
	}
}

int32 URaidBossInventoryWidget::GetConsumableItemAmount(int32 Index) const
{
	if (WeakInventorySystem != nullptr)
	{
		return WeakInventorySystem->GetItemAmount(EITemCategory::Consumable, Index);
	}

	return 0;
}

EITemCategory URaidBossInventoryWidget::GetShownInventory() const
{
	return ShownInventory;
}

const URaidBossEquipmentItem* URaidBossInventoryWidget::GetEquipmentItem(int32 Index) const
{
	if (WeakInventorySystem != nullptr)
	{
		return WeakInventorySystem->GetEquipmentItem(Index);
	}

	return nullptr;
}

void URaidBossInventoryWidget::SetWeakInventorySystem(URaidBossInventorySystem* InWeakInventorySystem)
{
	WeakInventorySystem = InWeakInventorySystem;
}

void URaidBossInventoryWidget::SetItemCategory(EITemCategory Category)
{
	ShownInventory = Category;
}

void URaidBossInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateCanTick();
}

void URaidBossInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInventorySlot();
	UpdateEquipmentSlot();
}

void URaidBossInventoryWidget::UpdateInventorySlot()
{
	if (WeakInventorySystem == nullptr)
		return;

	int32 MaximumAmount = WeakInventorySystem->GetMaximumItemAmount();
	
	for (int i = 0; i < MaximumAmount; i++)
	{
		const URaidBossItemBase* ItemCDO = WeakInventorySystem->GetItemCDO(ShownInventory, i);
		int32					 ItemAmount = WeakInventorySystem->GetItemAmount(ShownInventory, i);
		
		if (ItemCDO != nullptr)
		{
			UTexture2D*	Texture = ItemCDO->GetItemTexture();
			
			InventorySlots[i]->SetTexture(Texture);
			InventorySlots[i]->SetItemAmount(ItemAmount);
		}
		else
		{
			InventorySlots[i]->SetTexture(nullptr);
			InventorySlots[i]->SetItemAmount(0);
		}
	}
}

void URaidBossInventoryWidget::UpdateEquipmentSlot()
{
	int32 EquipTypeSize = static_cast<int32>(EEquipType::Size);
	
	for (int i = 0; i < EquipTypeSize; i++)
	{
		const URaidBossEquipmentItem*	Item = WeakInventorySystem->GetEquippedItem(i);
		UTexture2D*						Texture2D = Item ? Item->GetItemTexture() : nullptr;
		
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
