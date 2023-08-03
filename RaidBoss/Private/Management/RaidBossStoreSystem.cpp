#include "Management/RaidBossStoreSystem.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/RaidBossStoreWidget.h"

URaidBossStoreSystem::URaidBossStoreSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void URaidBossStoreSystem::BeginPlay()
{
	Super::BeginPlay();

	if (StoreWidget)
		StoreWidget->SetWeakStoreSystem(this);
}

void URaidBossStoreSystem::OpenStoreWidget()
{
	if (PlayerController && StoreWidget && StoreWidget->IsInViewport() == false)
	{
		FInputModeUIOnly InputMode;
		
		InventorySystem = PlayerController->GetInventorySystem();
		PlayerController->SetInputMode(InputMode);
		PlayerController->FlushPressedKeys();
		PlayerController->SetShowMouseCursor(true);
		
		InputMode.SetWidgetToFocus(StoreWidget->TakeWidget());
		StoreWidget->AddToViewportWithTracking();
	}
}

void URaidBossStoreSystem::CloseStoreWidget()
{
	if (StoreWidget && StoreWidget->IsInViewport() == true)
	{
		int32	RemainingWidget;
		
		StoreWidget->RemoveWidget(StoreWidget, RemainingWidget);
		if (RemainingWidget == 0)
		{
			FInputModeGameOnly	InputMode;
			
			PlayerController->SetInputMode(InputMode);
			PlayerController->FlushPressedKeys();
			PlayerController->SetShowMouseCursor(false);
		}
		else
		{
			FInputModeGameAndUI	InputMode;

			PlayerController->SetInputMode(InputMode);
		}
	}
}

bool URaidBossStoreSystem::RespondBuyingRequest(int32 ProductsIndex)
{
	URaidBossItemBase*	Product;

	if (Products.IsValidIndex(ProductsIndex) == false)
		return false;
	
	Product = Products[ProductsIndex].GetDefaultObject();

	if (Product == nullptr || InventorySystem == nullptr)
		return false;

	bool	bIsNotEnoughGold = (Product->GetItemInfo().BuyingPrice > InventorySystem->GetGold());
	bool	bIsInventoryFull = InventorySystem->IsInventoryFull(Product->GetItemCategory());
	
	if (bIsNotEnoughGold == false && bIsInventoryFull == false)
	{
		InventorySystem->SetGold(InventorySystem->GetGold() - Product->GetItemInfo().BuyingPrice);
		InventorySystem->AddNewItem(Product->GetClass());
	}
	
	return true;
}

bool URaidBossStoreSystem::RespondSellingRequest(EITemCategory Category, int32 InventoryIndex)
{
	const URaidBossItemBase* Item = InventorySystem->GetItemCDO(Category, InventoryIndex);

	if (Item)
	{
		InventorySystem->SetGold(InventorySystem->GetGold() + Item->GetItemInfo().SellingPrice);
		InventorySystem->DecreaseItemAmount(Category, InventoryIndex);

		return true;
	}

	return false;
}

const TArray<TSubclassOf<URaidBossItemBase>>& URaidBossStoreSystem::GetProducts() const
{
	return Products;
}

const int32& URaidBossStoreSystem::GetGoldFromPlayer() const
{
	return InventorySystem->GetGold();
}

const TArray<FItemInfomation>& URaidBossStoreSystem::GetEquipItemsFromPlayer() const
{
	return InventorySystem->GetEquipItems();
}

const TArray<FItemInfomation>& URaidBossStoreSystem::GetConsumableItemsFromPlayer() const
{
	return InventorySystem->GetConsumableItems();
}

int32 URaidBossStoreSystem::GetMaximumItemAmount() const
{
	if (InventorySystem != nullptr)
		return InventorySystem->GetMaximumItemAmount();

	return 0;
}

void URaidBossStoreSystem::SetPlayerController(ARaidBossPlayerControllerBase* InPlayerController)
{
	PlayerController = InPlayerController;
}
