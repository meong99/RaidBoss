// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "QuickSlot.generated.h"

class URaidBossSkillBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UQuickSlot : public URaidBossSlotWidget
{
	GENERATED_BODY()
public:
	UQuickSlot();
	
	UFUNCTION(BlueprintCallable)
	virtual void ActivateThisSlot() override;
	
	void	SetNewSkill(const URaidBossSkillBase* NewSkill);

	void	RemoveRegisteredSkill();
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	void	SetOtherSlot(URaidBossSlotWidget* OtherSlot);

	void	RemoveOtherSlot();
	
	void	SendGameplayEventWithTag(const FGameplayTag& TagToAdd = FGameplayTag{}) const;

protected:
	UPROPERTY()
	TObjectPtr<URaidBossSlotWidget>	OwningSlot;
};
