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

protected:
	virtual void	NativeConstruct() override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual void	ActivateThisSlot() {}
	virtual void	SetItemAmount(IN int32 Amount) { ItemAmount = Amount; }

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	UTexture2D*			GetTexture() const;
	ESlotType			GetSlotType() const { return SlotType; }
	int32				GetItemAmount() const { return ItemAmount; }
	const FGameplayTag&	GetAbilityTriggerTag() const { return AbilityTriggerTag; }
	
	void	SetTexture(IN UTexture2D* InTexture);
	void	SetSlotType(IN ESlotType InSlotType) { SlotType = InSlotType; }
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UImage>		BindImage;
	
	UPROPERTY(EditInstanceOnly, Category="Raid Boss | Slot Widget")
	TObjectPtr<UTexture2D>	DefaultTexture;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	ESlotType				SlotType = ESlotType::None;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	EDragPivot				DragPivot = EDragPivot::CenterCenter;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget")
	FVector2D				DragOffset = FVector2d(0, 0);
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	FGameplayTag	AbilityTriggerTag;

	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	int32			ItemAmount;

};
