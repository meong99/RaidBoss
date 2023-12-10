// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionalUI.h"
#include "Components/VerticalBox.h"
#include "SkillUI.generated.h"

class USkillSlot;
class URaidBossSkillBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API USkillUI : public UInteractionalUI
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// 플레이어의 모든 스킬을 UI에 등록
	void	RegisterAllSkillsToUI();
	
protected:
	void	CreateAndInitSlots(const TArray<TSubclassOf<URaidBossSkillBase>>* PlayerSkills) const;

	bool	InitSlotBySkill(URaidBossSkillBase* SkillCDO, USkillSlot* NewSlot) const;

	const TArray<TSubclassOf<URaidBossSkillBase>>* GetPlayerSkills() const;

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Slot Widget")
	TSubclassOf<USkillSlot>		SkillSlotClass;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Slot Widget", meta=(BindWidget))
	TObjectPtr<UVerticalBox>	SkillVerticalBox;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//

};
