// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "InventorySlot.generated.h"

class URaidBossConsumableItem;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UInventorySlot : public URaidBossSlotWidget
{
	GENERATED_BODY()

public:
	virtual void ActivateThisSlot() override;
	void	RegisterNewItem(FGameplayTag InAbilityTriggerTag, int32 Amount, UTexture2D* NewItemImage);
	
protected:
	void	TryActivateThisSlotAbility() const;
	virtual void	NativeOnInitialized() override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void	NotifyItemAmountCallBack(FGameplayTag InAbilityTriggerTag, int32 Amount);
	void	ResetSlot();

	void	AddDynamicCallBackFunction();
	void	RemoveDynamicCallBackFunction();
private:
	void	SwapInventorySlot(UInventorySlot* OtherSlot);
};
