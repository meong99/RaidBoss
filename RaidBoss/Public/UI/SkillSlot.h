// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SkillSlot.generated.h"

class URaidBossSkillBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API USkillSlot : public URaidBossSlotWidget
{
	GENERATED_BODY()
	
protected:
	virtual void	NativeOnInitialized() override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	virtual void ActivateThisSlot() override;

	UFUNCTION()
	void	NotifySkillLevelChangedCallback(FGameplayTag InAbilityTriggerTag, int32 InCurrentSkillLevel);
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Action")
	void	IncreaseSkillLevel();
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Action")
	void	DecreaseSkillLevel();
	
	void	InitSlot(FGameplayTag InAbilityTriggerTag, UTexture2D* InSkillTexture, FString InSkillName, int32 InRequirePoint, int32 InSkillLevel);

protected:
	void	TryActivateAbilityInSlot() const;
	
	//스킬 레벨 변경에 대한 델리게이트를 등록
	void	AddSkillLevelMonitoring() const;
	
	//스킬 레벨 변경에 대한 델리게이트를 등록 해제
	void	RemoveSkillLevelMonitoring() const;

	void	SendGameplayEventWithTag(const FGameplayTag& TagToAdd = FGameplayTag{}) const;

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UTextBlock>	CurrentSkillName;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UTextBlock>	RequirePoint;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UTextBlock>	CurrentSkillLevel;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UButton>		LevelUpButton;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UButton>		LevelDownButton;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//

};
