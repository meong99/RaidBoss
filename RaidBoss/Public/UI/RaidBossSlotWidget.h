#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "RaidBossSlotWidget.generated.h"

class UInteractionalUIAction;
class UImage;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ConsumableSlot UMETA(DisplayName = "ItemSlot"),
	EquipmentSlot UMETA(DisplayName = "EquipSlot"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class RAIDBOSS_API URaidBossSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void	ActivateThisSlot();

	
	UTexture2D*	GetTexture() const;
	ESlotType	GetSlotType() const { return SlotType; }
	int32		GetItemAmount() const;
	const FGameplayTag&	GetAbilityTriggerTag() const { return AbilityTriggerTag; }
	
	virtual void	SetItemAmount(IN int32 Amount);
	void	SetTexture(IN UTexture2D* InTexture);
	void	SetSlotType(IN ESlotType InSlotType);
	
protected:
	virtual void	NativeConstruct() override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	FGameplayTag				AbilityTriggerTag;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage>			BindImage;
	UPROPERTY(EditInstanceOnly, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	TObjectPtr<UTexture2D>		DefaultTexture;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	int32						ItemAmount;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	ESlotType					SlotType = ESlotType::None;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	EDragPivot					DragPivot = EDragPivot::CenterCenter;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(AllowPrivateAccess))
	FVector2D					DragOffset = FVector2d(0, 0);
};
