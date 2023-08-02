#include "UI/RaidBossStoreWidget.h"
#include "Management/RaidBossStoreSystem.h"
#include "UI/RaidBossStoreSlotWidget.h"

void URaidBossStoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInventory();
}

void URaidBossStoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitInventoryData();
	UpdateCanTick();
	CreateProductSlots();
	CreateInventorySlots();
}

void URaidBossStoreWidget::NativeDestruct()
{
	Super::NativeDestruct();

	StoreScroll->ClearChildren();
	InventoryScroll->ClearChildren();
}

void URaidBossStoreWidget::RespondBuyingRequest(int32 ProductsIndex)
{
	if (WeakStoreSystem != nullptr)
	{
		WeakStoreSystem->RespondBuyingRequest(ProductsIndex);
	}
}

void URaidBossStoreWidget::RespondSellingRequest(int32 InventoryIndex)
{
	if (WeakStoreSystem != nullptr)
	{
		WeakStoreSystem->RespondSellingRequest(ShownItemCategory, InventoryIndex);
	}
}

void URaidBossStoreWidget::InitInventoryData()
{
	if (WeakStoreSystem != nullptr)
	{
		EquipItems = &WeakStoreSystem->GetEquipItemsFromPlayer();
		ConsumableItems = &WeakStoreSystem->GetConsumableItemsFromPlayer();
	}
}

void URaidBossStoreWidget::CreateProductSlots()
{
	if (WeakStoreSystem == nullptr)
		return;
	
	const TArray<TSubclassOf<URaidBossItemBase>>& Products = WeakStoreSystem->GetProducts();

	for (int i = 0; i < Products.Num(); i++)
	{
		URaidBossItemBase*			Item = Cast<URaidBossItemBase>(Products[i]->GetDefaultObject());
		URaidBossStoreSlotWidget*	SlotWidget = Cast<URaidBossStoreSlotWidget>(CreateWidget(this, SlotClass));

		if (Item && SlotWidget)
		{
			SlotWidget->SetWeakStoreWidget(this);
			SlotWidget->SetItemTexture(Item->GetItemTexture());
			SlotWidget->SetIndex(i);
			SlotWidget->SetIsProduct(true);
			SlotWidget->SetItemAmount(0);
			SlotWidget->SetItemName(Item->GetItemInfo().ItemName.ToString());
			SlotWidget->SetItemPrice(Item->GetItemInfo().SellingPrice);
			StoreScroll->AddChild(SlotWidget);
		}
	}
}

void URaidBossStoreWidget::CreateInventorySlots()
{
	if (WeakStoreSystem == nullptr)
		return;
	
	int32 MaximumItemAmount = WeakStoreSystem->GetMaximumItemAmount();

	for (int i = 0; i < MaximumItemAmount; i++)
	{
		URaidBossStoreSlotWidget*	SlotWidget = Cast<URaidBossStoreSlotWidget>(CreateWidget(this, SlotClass));

		if (SlotWidget)
		{
			SlotWidget->SetWeakStoreWidget(this);
			SlotWidget->SetIsProduct(false);
			SlotWidget->SetItemAmount(0);
			InventoryScroll->AddChild(SlotWidget);
		}
	}
}

void URaidBossStoreWidget::UpdateInventory()
{
	if (WeakStoreSystem == nullptr)
		return;
	
	const TArray<FItemInfomation>&	CurrentBelonging = GetCurrentBelonging();
	TArray<UWidget*>				Slots = InventoryScroll->GetAllChildren();

	GoldPlayerHas = WeakStoreSystem->GetGoldFromPlayer();
	
	if (CurrentBelonging.Num() != Slots.Num())
		return;
	
	for (int i = 0; i < CurrentBelonging.Num(); i++)
	{
		const URaidBossItemBase*	Item = CurrentBelonging[i].GetItemCDO();
		URaidBossStoreSlotWidget*	SlotWidget = Cast<URaidBossStoreSlotWidget>(Slots[i]);

		if (Item && SlotWidget)
		{
			SlotWidget->SetItemTexture(Item->GetItemTexture());
			SlotWidget->SetIndex(i);
			SlotWidget->SetItemAmount(CurrentBelonging[i].Amount);
			SlotWidget->SetItemName(Item->GetItemInfo().ItemName.ToString());
			SlotWidget->SetItemPrice(Item->GetItemInfo().SellingPrice);
		}
		else if (Item == nullptr)
		{
			SlotWidget->SetItemTexture(nullptr);
			SlotWidget->SetItemAmount(0);
			SlotWidget->SetItemName("");
			SlotWidget->SetItemPrice(0);
		}
	}
}

void URaidBossStoreWidget::SetShownItemCategory(EITemCategory Category)
{
	ShownItemCategory = Category;
}

void URaidBossStoreWidget::SetWeakStoreSystem(URaidBossStoreSystem* InWeakStoreSystem)
{
	WeakStoreSystem = InWeakStoreSystem;
}

const TArray<FItemInfomation>& URaidBossStoreWidget::GetCurrentBelonging()
{
	if (ShownItemCategory == EITemCategory::Equip)
		return *EquipItems;

	return *ConsumableItems;
}
