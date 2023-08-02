#pragma once

#include "CoreMinimal.h"
#include "RaidBossUserWidgetBase.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Components/ScrollBox.h"
#include "Management/RaidBossInventorySystem.h"
#include "RaidBossStoreWidget.generated.h"

class URaidBossStoreSlotWidget;
class URaidBossStoreSystem;

UCLASS()
class RAIDBOSS_API URaidBossStoreWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

/*
*	----------- Override
*/
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	void	RespondBuyingRequest(int32 ProductsIndex);
	void	RespondSellingRequest(int32 InventoryIndex);
private:
	void	InitInventoryData();
	void	CreateProductSlots();
	void	CreateInventorySlots();
	void	UpdateInventory();
/*
*	----------- Access(Get, Set, Check)
*/
public:
	void	SetWeakStoreSystem(URaidBossStoreSystem* InWeakStoreSystem);
private:
	const TArray<FItemInfomation>&	GetCurrentBelonging();
/*
*	----------- Member Variables
*/
private:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Widget", meta=(AllowPrivateAccess))
	TWeakObjectPtr<URaidBossStoreSystem>	WeakStoreSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Store Widget", meta=(AllowPrivateAccess))
	TSubclassOf<URaidBossStoreSlotWidget>	SlotClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Store Widget", meta=(AllowPrivateAccess))
	EITemCategory							ShownItemCategory = EITemCategory::Equip;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Store Widget", meta=(AllowPrivateAccess))
	int32									GoldPlayerHas;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Store Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox>	StoreScroll;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Store Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox>	InventoryScroll;

	const TArray<FItemInfomation>* EquipItems;
	const TArray<FItemInfomation>* ConsumableItems;
};
