#include "UI/RaidBossStoreSlotWidget.h"

void URaidBossStoreSlotWidget::BuyItem()
{
	if (WeakStoreWidget != nullptr)
	{
		WeakStoreWidget->RespondBuyingRequest(Index);
	}
}

void URaidBossStoreSlotWidget::SellItem()
{
	if (WeakStoreWidget != nullptr)
	{
		WeakStoreWidget->RespondSellingRequest(Index);
	}
}

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

void URaidBossStoreSlotWidget::SetIsProduct(bool InIsProduct)
{
	bIsProduct = InIsProduct;
}
