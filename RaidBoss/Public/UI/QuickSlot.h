// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void ActivateThisSlot() override;
	
	void	SetNewSkill(const URaidBossSkillBase* NewSkill);

	void	RemoveRegisteredSkill();
	
protected:
	void	SetOtherSlot(URaidBossSlotWidget* OtherSlot);

	void	RemoveOtherSlot();
	
	void	SendGameplayEventWithTag(const FGameplayTag& TagToAdd = {});

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
	UPROPERTY(Transient, BlueprintReadOnly, Category="Raid Boss | QuickSlot", meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>	CooldownFinishedAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | QuickSlot", meta=(BindWidget))
	TObjectPtr<UProgressBar>		CooldownBar;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	// 스킬 쿨타임을 보여주는 텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | QuickSlot", meta=(BindWidget))
	TObjectPtr<UTextBlock>			CoolTime;

	// 해당 퀵슬롯에 등록된 다른 슬롯(스킬, 인벤토리)
	UPROPERTY()
	TObjectPtr<URaidBossSlotWidget>	OwningSlot;

	// 현재 슬롯의 남은 쿨타임
	float	RemainingCoolDown = 0;

	// 현재 슬롯의 전체 쿨타임
	float	OriginCoolDown = 0;

	bool	bShouldPlayFinishAnimation = false;
};
