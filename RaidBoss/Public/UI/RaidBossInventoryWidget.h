#pragma once

#include "CoreMinimal.h"
#include "UI/RaidBossUserWidgetBase.h"
#include "UI/RaidBossSlotWidget.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Management/RaidBossInventorySystem.h"
#include "RaidBossInventoryWidget.generated.h"

class URaidBossEquipmentItem;
class URaidBossInventorySystem;
class URaidBossSlotWidget;
class UButton;
class UScrollBox;
class UWrapBox;

UCLASS()
class RAIDBOSS_API URaidBossInventoryWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual bool	Initialize() override;

	UFUNCTION()
	void	ClickedEquipButton();
	UFUNCTION()
	void	ClickedConsumableButton();
	UFUNCTION()
	void	ClickedMiscellaneousButton();

	void	CreateSlot();
	void	CreateInventorySlot();
	void	InitEquipmentSlot();
	
	void	EquipItem(int32 Index) const;
	void	UnEquipItem(int32 Index) const;
	void	UseConsumableItem(int32 Index) const;
	void	UseItem(ESlotType SlotType, int32 Index) const;
	void	ChangeItemOrder(int32 Index1, int32 Index2) const;
	int32	GetConsumableItemAmount(int32 Index) const;

	EITemCategory					GetShownInventory() const;
	const URaidBossEquipmentItem*	GetEquipmentItem(int32 Index) const;
	
	void	SetWeakInventorySystem(IN URaidBossInventorySystem* InWeakInventorySystem);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Inventory Widget")
	void	SetItemCategory(EITemCategory Category);

protected:
	virtual void	NativeConstruct() final;
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) final;
	
private:
	void	UpdateInventorySlot();
	void	UpdateEquipmentSlot();

private:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta=(AllowPrivateAccess))
	TWeakObjectPtr<URaidBossInventorySystem>	WeakInventorySystem;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta = (ExposeOnSpawn = true, AllowPrivateAccess))
	EITemCategory								ShownInventory = EITemCategory::Equip;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta = (ExposeOnSpawn = true, AllowPrivateAccess))
	TSubclassOf<URaidBossSlotWidget>			InventorySlotClass;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Inventory Widget", meta=(AllowPrivateAccess))
	TArray<URaidBossSlotWidget*>				InventorySlots;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox>	InventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWrapBox>	SlotWrapBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton>		EquipButton;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton>		ConsumableButton;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Inventory Widget", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton>		MiscellaneousButton;

	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<URaidBossSlotWidget>	HelmetSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<URaidBossSlotWidget>	WeaponSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<URaidBossSlotWidget>	ArmorTopSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<URaidBossSlotWidget>	ArmorBottomSlot;
	UPROPERTY(BlueprintReadOnly, Category="Equipment Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<URaidBossSlotWidget>	ArmorShoesSlot;
};
