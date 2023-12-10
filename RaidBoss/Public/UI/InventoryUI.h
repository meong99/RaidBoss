// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionalUI.h"
#include "RaidBossSlotWidget.h"
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
	UInventoryUI();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void	NotifyNewItemAddedCallBack(URaidBossItemBase* NewItemCDO, int32 Amount);

	void	CreateItemSlots(TArray<UWidget*> InventoryWrapBox, ESlotType SlotType);

	void	AddNewItemToSlot(TArray<UWidget*> InventoryWrapBox, URaidBossItemBase* NewItemCDO, int32 Amount) const;
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Inventory Widget")
	TSubclassOf<UInventorySlot>	InventorySlotClass;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWrapBox>		EquipmentWrapBox;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWrapBox>		ConsumableWrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Inventory Widget")
	int32						SlotCount = 20;
	
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
