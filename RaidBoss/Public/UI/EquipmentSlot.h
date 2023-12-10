// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "EquipmentSlot.generated.h"

class URaidBossConsumableItem;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UEquipmentSlot : public URaidBossSlotWidget
{
	GENERATED_BODY()

protected:
	virtual void	NativeOnInitialized() override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION()
	void	NotifyEquipmentChangedCallback(FGameplayTag InAbilityTriggerTag, int32 InEquipmentType, UTexture2D* NewItemImage);
	
	void	EquipItem(FGameplayTag InAbilityTriggerTag, UTexture2D* NewItemImage);
	
	void	UnEquipItem();

	void	RetriggerAbilityForUnEquipItem() const;

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	TObjectPtr<UImage>		BackgroundImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	EEquipType				EquipmentType;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
