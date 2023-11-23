// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionalUI.h"
#include "InventoryUI.generated.h"

class URaidBossConsumableItem;
class UWrapBox;
class URaidBossItemBase;
class UInventorySlot;

UCLASS()
class RAIDBOSS_API UInventoryUI : public UInteractionalUI
{
	GENERATED_BODY()

public:
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void	NotifyNewItemAddedCallBack(URaidBossItemBase* NewItemCDO, int32 Amount);

	void	AddNewItemToSlot(TArray<UWidget*> InventorySlots, URaidBossItemBase* NewItemCDO, int32 Amount) const;
protected:
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWrapBox>	EquipmentWrapBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWrapBox>	ConsumableWrapBox;
	// UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	// TObjectPtr<UWrapBox>	MiscellaneousWrapBox;
};
