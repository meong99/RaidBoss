// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "Components/TextBlock.h"
#include "InventorySlot.generated.h"

class URaidBossItemBase;
class URaidBossConsumableItem;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UInventorySlot : public URaidBossSlotWidget
{
    GENERATED_BODY()

protected:
    virtual bool    NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation) override;
    virtual void    NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                         UDragDropOperation*& OutOperation) override;
    virtual FReply  NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
    virtual void    ActivateThisSlot() override;

    UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | Inventory")
    void    ObtainNewItem(const URaidBossItemBase* NewItemCDO);

    void    RegisterNewItem(const URaidBossItemBase* NewItemCDO, int32 Amount);

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss | Inventory")
    const URaidBossItemBase*    GetCurrentItem() const { return CurrentItem.Get(); }

protected:
    virtual void    SetItemAmount(int32 Amount) override;

    UFUNCTION()
    void    NotifyItemAmountCallBack(FGameplayTag InAbilityTriggerTag, int32 Amount);

    void    TryActivateThisSlotAbility() const;

    void    ResetSlot();

    void    AddDynamicCallBackFunction();

    void    RemoveDynamicCallBackFunction();

    void    SwapInventorySlot(UInventorySlot* OtherSlot);

    bool    WasAbilityActivated(const URaidBossItemBase* NewItemCDO) const;

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Slot", meta=(BindWidget, AllowPrivateAccess))
    TObjectPtr<UTextBlock> AmountText;

    TWeakObjectPtr<const URaidBossItemBase> CurrentItem;
};
