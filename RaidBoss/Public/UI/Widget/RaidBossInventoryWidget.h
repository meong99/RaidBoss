#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Blueprint/UserWidget.h"
#include "RaidBossInventoryWidget.generated.h"

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
/*
*	----------- Process Method
*/
public:
	void	CreateSlots();
	void	ResetSlotImage();

	UFUNCTION(BlueprintImplementableEvent)
	void	WidgetOnVisible();
	UFUNCTION(BlueprintImplementableEvent)
	void	WidgetOnHidden();
	UFUNCTION()
	void	ClickedEquipButton();
	UFUNCTION()
	void	ClickedConsumableButton();
	UFUNCTION()
	void	ClickedMiscellaneousButton();
	UFUNCTION(BlueprintCallable)
	void	UpdateInventorySlot();
	UFUNCTION(BlueprintCallable)
	void	ChangeItemOrder(int32 Index1, int32 Index2);
/*
*	----------- Access(Get, Set, Check)
*/
	void	SetWeakInventorySystem(IN URaidBossInventorySystem* InWeakInventorySystem);
	
	UFUNCTION(BlueprintCallable)
	void	SetItemCategory(EITemCategory Category);
/*
*	----------- Member Variables
*/

public:
	UPROPERTY(BlueprintReadOnly, Category="Item")
	TWeakObjectPtr<URaidBossInventorySystem>	WeakInventorySystem;
	
	UPROPERTY(BlueprintReadOnly, Category="Item", meta = (ExposeOnSpawn = true))
	EITemCategory								ItemCategory = EITemCategory::Equip;
	UPROPERTY(BlueprintReadOnly, Category="Widget", meta = (ExposeOnSpawn = true))
	TSubclassOf<URaidBossSlotWidget>			SlotClass;
	UPROPERTY(BlueprintReadOnly, Category="Widget")
	TArray<URaidBossSlotWidget*>				ItemSlots;
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta = (BindWidget))
	TObjectPtr<UScrollBox>						InventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta = (BindWidget))
	TObjectPtr<UWrapBox>						SlotWrapBox;
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta = (BindWidget))
	TObjectPtr<UButton>							EquipButton;
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta = (BindWidget))
	TObjectPtr<UButton>							ConsumableButton;
	UPROPERTY(BlueprintReadWrite, Category="Widget", meta = (BindWidget))
	TObjectPtr<UButton>							MiscellaneousButton;
	
};
