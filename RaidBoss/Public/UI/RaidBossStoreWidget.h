#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Components/ScrollBox.h"
#include "Management/RaidBossInventorySystem.h"
#include "RaidBossUserWidgetBase.h"
#include "RaidBossStoreWidget.generated.h"

class URaidBossStoreSlotWidget;
class URaidBossStoreSystem;

UCLASS()
class RAIDBOSS_API URaidBossStoreWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

public:
	void	RespondBuyingRequest(int32 ProductsIndex) const;
	void	RespondSellingRequest(int32 InventoryIndex) const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store Widget")
	void	SetShownItemCategory(EITemCategory Category);
	void	SetWeakStoreSystem(IN URaidBossStoreSystem* InWeakStoreSystem);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	void	InitInventoryData();
	void	CreateProductSlots();
	void	CreateInventorySlots();
	void	UpdateInventory();

	const TArray<FItemInformation>&	GetCurrentBelonging() const;

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

	const TArray<FItemInformation>* EquipItems;
	const TArray<FItemInformation>* ConsumableItems;
};
