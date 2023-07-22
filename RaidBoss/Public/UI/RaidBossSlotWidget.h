#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "RaidBossSlotWidget.generated.h"

class UImage;
class URaidBossAbilityBase;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ItemSlot UMETA(DisplayName = "ItemSlot"),
	EquipmentSlot UMETA(DisplayName = "EquipSlot"),
	SkillSlot UMETA(DisplayName = "SkillSlot"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class RAIDBOSS_API URaidBossSlotWidget : public UUserWidget
{
	GENERATED_BODY()
/*
*	----------- Override
*/
protected:
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
protected:
	void	DropOnItemSlot(URaidBossSlotWidget* Payload);
	void	DropOnEquipmentSlot(URaidBossSlotWidget* Payload);
	bool	IsEquippable(URaidBossSlotWidget* Payload);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	void	SetWeakOwnerWidget(IN UUserWidget* InWeakOwnerWidget);
	void	SetTexture(IN UTexture2D* InTexture);
	void	SetIndex(int32 InIndex);
	void	SetSlotType(ESlotType InSlotType);
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	TWeakObjectPtr<UUserWidget>	WeakOwnerWidget;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	ESlotType					SlotType = ESlotType::None;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	int32						Index = 0;

	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UImage>		BindImage;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	TObjectPtr<UTexture2D>	Texture;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	EDragPivot				DragPivot = EDragPivot::CenterCenter;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	FVector2D				DragOffset = FVector2d(0, 0);
};
