// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionalUI.h"
#include "QuickSlotUI.generated.h"

class URaidBossSkillBase;
class UQuickSlot;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UQuickSlotUI : public UInteractionalUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void	NotifyNewWeaponEquippedCallBack(const TArray<URaidBossSkillBase*>& NewSkills);
	
protected:
	void	ResetAllSkillSlots();
	
	void	RegisterNewSKills(const TArray<URaidBossSkillBase*>& NewSkills);
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UQuickSlot>	QuickSlot_Q;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UQuickSlot>	QuickSlot_E;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UQuickSlot>	QuickSlot_R;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UQuickSlot>	QuickSlot_Right;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
