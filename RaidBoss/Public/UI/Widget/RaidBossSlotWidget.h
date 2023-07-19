#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaidBossSlotWidget.generated.h"

class UImage;
class URaidBossAbilityBase;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ItemSlot UMETA(DisplayName = "ItemSlot"),
	EquipSlot UMETA(DisplayName = "EquipSlot"),
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
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
	void	DropOnItemSlot(URaidBossSlotWidget* Payload);
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
	TWeakObjectPtr<UUserWidget>		WeakOwnerWidget;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage>				BindImage;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ESlotType						SlotType = ESlotType::None;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D>			Texture;
	UPROPERTY(BlueprintReadOnly)
	int32							Index = 0;
};
