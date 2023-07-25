#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/RaidBossSlotWidget.h"
#include "RaidBossInventoryWidget.generated.h"

class URaidBossEquipmentItem;
class UButton;
class URaidBossInventorySystem;
class URaidBossSlotWidget;
class UScrollBox;
class UWrapBox;

UCLASS()
class RAIDBOSS_API URaidBossInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URaidBossInventoryWidget(const FObjectInitializer& Initializer);
/*
*	----------- Override
*/
	virtual bool Initialize() override;
/*
*	----------- Used on delegate
*/
	UFUNCTION()
	void	ClickedEquipButton();
	UFUNCTION()
	void	ClickedConsumableButton();
	UFUNCTION()
	void	ClickedMiscellaneousButton();
/*
*	----------- Process Method
*/
public:
	void	CreateSlot();
	void	CreateInventorySlot();
	void	InitEquipmentSlot();
	void	UpdateInventorySlot();
	void	UpdateEquipmentSlot();
	void	ResetInventorySlotImage();
	void	EquipItem(int32 Index);
	void	UnEquipItem(int32 Index);
	void	UseItem(ESlotType SlotType, int32 Index);
	void	ChangeItemOrder(int32 Index1, int32 Index2);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	EITemCategory					GetShownInventory() const;
	const URaidBossEquipmentItem*	GetEquipmentItem(int32 Index) const;
	
	void	SetWeakInventorySystem(IN URaidBossInventorySystem* InWeakInventorySystem);
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Inventory Widget")
	void	SetItemCategory(EITemCategory Category);
/*
*	----------- Member Variables
*/

protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget")
	TWeakObjectPtr<URaidBossInventorySystem>	WeakInventorySystem;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta = (ExposeOnSpawn = true))
	EITemCategory								ShownInventory = EITemCategory::Equip;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta = (ExposeOnSpawn = true))
	TSubclassOf<URaidBossSlotWidget>			InventorySlotClass;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget")
	TArray<URaidBossSlotWidget*>				InventorySlots;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget))
	TObjectPtr<UScrollBox>						InventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget))
	TObjectPtr<UWrapBox>						SlotWrapBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget))
	TObjectPtr<UButton>							EquipButton;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget))
	TObjectPtr<UButton>							ConsumableButton;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget))
	TObjectPtr<UButton>							MiscellaneousButton;

	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget))
	TObjectPtr<URaidBossSlotWidget>				HelmetSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget))
	TObjectPtr<URaidBossSlotWidget>				WeaponSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget))
	TObjectPtr<URaidBossSlotWidget>				ArmorTopSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget))
	TObjectPtr<URaidBossSlotWidget>				ArmorBottomSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget))
	TObjectPtr<URaidBossSlotWidget>				ArmorShoesSlot;
};
