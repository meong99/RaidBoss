#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "RaidBossSlotWidget.generated.h"

struct FItemInfomation;
class UImage;
class URaidBossAbilityBase;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ItemSlot UMETA(DisplayName = "ItemSlot"),
	EquipmentSlot UMETA(DisplayName = "EquipSlot"),
	SkillSlot UMETA(DisplayName = "SkillSlot"),
	QuickSlot UMETA(DisplayName = "QuickSlot"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class RAIDBOSS_API URaidBossSlotWidget : public UUserWidget
{
	GENERATED_BODY()
/*
*	----------- Override
*/
public:
	virtual void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	virtual void	NativeConstruct() override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	UFUNCTION(BlueprintCallable)
	void	UseQuickSlot();
protected:
	void	DropOnItemSlot(const URaidBossSlotWidget* Payload);
	void	DropOnEquipmentSlot(const URaidBossSlotWidget* Payload);
	void	DropOnQuickSlot(const URaidBossSlotWidget* Payload);
	bool	IsEquippable(const URaidBossSlotWidget* Payload) const;
/*
*	----------- Access(Get, Set, Check)
*/
public:
	void	SetWeakOwnerWidget(IN UUserWidget* InWeakOwnerWidget);
	void	SetTexture(IN UTexture2D* InTexture);
	void	SetIndex(IN int32 InIndex);
	void	SetItemAmount(IN int32 Amount);
	void	SetSlotType(IN ESlotType InSlotType);
/*
*	----------- Member Variables
*/
private:
	TObjectPtr<const URaidBossSlotWidget>	RefPayload;
	
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	TWeakObjectPtr<UUserWidget>	WeakOwnerWidget;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	int32						Index = 0;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	FString						ItemAmount;

	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage>		BindImage;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	ESlotType				SlotType = ESlotType::None;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	EDragPivot				DragPivot = EDragPivot::CenterCenter;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	FVector2D				DragOffset = FVector2d(0, 0);
};
