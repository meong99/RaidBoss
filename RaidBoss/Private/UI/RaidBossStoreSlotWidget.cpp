#include "UI/RaidBossStoreSlotWidget.h"

void URaidBossStoreSlotWidget::SetWeakStoreWidget(URaidBossStoreWidget* InWeakStoreWidget)
{
	WeakStoreWidget = InWeakStoreWidget;
}

void URaidBossStoreSlotWidget::SetItemTexture(UTexture2D* Texture)
{
	ItemImage->SetBrushFromTexture(Texture);
}

void URaidBossStoreSlotWidget::SetIndex(int32 InIndex)
{
	Index = InIndex;
}

void URaidBossStoreSlotWidget::SetItemAmount(int32 InAmount)
{
	if (InAmount == 0)
	{
		ItemAmount = "";
	}
	else
	{
		ItemAmount = FString::FromInt(InAmount);
	}
}

void URaidBossStoreSlotWidget::SetItemName(FString InItemName)
{
	ItemName = InItemName;
}

void URaidBossStoreSlotWidget::SetItemPrice(int32 InItemPrice)
{
	if (InItemPrice == 0)
	{
		ItemPrice = "";
	}
	else
	{
		ItemPrice = FString::FromInt(InItemPrice);
	}
}

void URaidBossStoreSlotWidget::SetIsProduct(bool bInIsProduct)
{
	bIsProduct = bInIsProduct;
}

void URaidBossStoreSlotWidget::BuyItem() const
{
	if (WeakStoreWidget != nullptr)
	{
		WeakStoreWidget->RespondBuyingRequest(Index);
	}
}

void URaidBossStoreSlotWidget::SellItem() const
{
	if (WeakStoreWidget != nullptr)
	{
		WeakStoreWidget->RespondSellingRequest(Index);
	}
}
