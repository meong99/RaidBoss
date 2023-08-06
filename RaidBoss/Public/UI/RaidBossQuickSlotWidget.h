#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "RaidBossUserWidgetBase.h"
#include "Components/HorizontalBox.h"
#include "RaidBossQuickSlotWidget.generated.h"

class URaidBossSkillWidget;
class URaidBossInventoryWidget;

USTRUCT()
struct FQuickSlotData
{
	GENERATED_BODY()

	FQuickSlotData()
		: SlotType(ESlotType::None)
		, AbilityIndex(-1)
		{}
	
	FQuickSlotData(ESlotType InSlotType, int32 InAbilityIndex)
		: SlotType(InSlotType)
		, AbilityIndex(InAbilityIndex)
		{}

	ESlotType	SlotType;
	int32		AbilityIndex;
};

UCLASS()
class RAIDBOSS_API URaidBossQuickSlotWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Quick Slot Widget")
	void	UseQuickSlot(int32 Index);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Quick Slot Widget")
	void	ResetQuickSlot(int32 Index);
	void	RegisterItemKey(int32 SlotIndex, ESlotType SlotType, int32 AbilityIndex);
	void	MoveSlotData(int32 Source, int32 Destination);
	
	ESlotType	GetRegisteredSlotType(int32 Index) const;
	
	void	SetInventoryWidget(IN URaidBossInventoryWidget* InInventoryWidget);
	void	SetSkillWidget(IN URaidBossSkillWidget* InSkillWidget);

protected:
	virtual void	NativeOnInitialized() final;
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) final;
	
private:
	TMap<int32, FQuickSlotData>				RegisteredDataMap;
	TObjectPtr<URaidBossInventoryWidget>	InventoryWidget;
	TObjectPtr<URaidBossSkillWidget>		SkillWidget;
	
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Quick Slot Widget", meta=(AllowPrivateAccess))
	TArray<URaidBossSlotWidget*>		SlotWidgets;
	UPROPERTY(EditInstanceOnly, Category="Raid Boss | Quick Slot Widget", meta=(AllowPrivateAccess))
	TSubclassOf<URaidBossSlotWidget>	SlotClass;
	UPROPERTY(EditInstanceOnly, Category="Raid Boss | Quick Slot Widget", meta=(AllowPrivateAccess))
	int32								SlotCount = 7;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Quick Slot Widget", meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UHorizontalBox>		QuickSlotBox;
};
