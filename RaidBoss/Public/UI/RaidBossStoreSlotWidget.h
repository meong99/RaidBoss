#pragma once

#include "CoreMinimal.h"
#include "RaidBossStoreWidget.h"
#include "RaidBossUserWidgetBase.h"
#include "Components/Image.h"
#include "RaidBossStoreSlotWidget.generated.h"

class URaidBossStoreWidget;

UCLASS()
class RAIDBOSS_API URaidBossStoreSlotWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

/*
*	----------- Override
*/
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store Slot Widget")
	void	BuyItem();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store Slot Widget")
	void	SellItem();
/*
*	----------- Access(Get, Set, Check)
*/
public:
	void	SetWeakStoreWidget(IN URaidBossStoreWidget*	InWeakStoreWidget);
	void	SetItemTexture(IN UTexture2D* Texture);
	void	SetIndex(int32 InIndex);
	void	SetItemAmount(int32 InAmount);
	void	SetItemName(FString InItemName);
	void	SetItemPrice(int32 InItemPrice);
	void	SetIsProduct(bool InIsProduct);
/*
*	----------- Member Variables
*/
private:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	TWeakObjectPtr<URaidBossStoreWidget>	WeakStoreWidget;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	int32	Index = -1;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	FString	ItemName;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	FString	ItemPrice;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	FString	ItemAmount;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess))
	bool	bIsProduct;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Store Slot Widget", meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UImage>	ItemImage;
};
