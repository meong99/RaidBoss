// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SkillUI.h"

#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/SkillSlot.h"

void USkillUI::RegisterAllSkillsToUI()
{
	CreateAndInitSlots(GetPlayerSkills());
}

void USkillUI::NativeOnInitialized()
{
	RegisterAllSkillsToUI();
}

void USkillUI::NativeConstruct()
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());

	if (ControllerBase)
	{
		ControllerBase->SetInputMode(FInputModeGameAndUI{});
		ControllerBase->SetShowMouseCursor(true);
	}
}

void USkillUI::NativeDestruct()
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());

	if (ControllerBase)
	{
		ControllerBase->SetInputMode(FInputModeGameOnly{});
		ControllerBase->SetShowMouseCursor(false);
	}
}

const TArray<TSubclassOf<URaidBossSkillBase>>* USkillUI::GetPlayerSkills() const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		return CharacterBase->GetPlayerSkills();
	}

	return nullptr;
}

void USkillUI::CreateAndInitSlots(const TArray<TSubclassOf<URaidBossSkillBase>>* PlayerSkills) const
{
	if (PlayerSkills == nullptr)
	{
		return;
	}
	
	for (int i = 0; i < PlayerSkills->Num(); i++)
	{
		URaidBossSkillBase* SkillCDO = (*PlayerSkills)[i]->GetDefaultObject<URaidBossSkillBase>();
		USkillSlot*			NewSlot = Cast<USkillSlot>(CreateWidget(GetOwningPlayer(), SkillSlotClass));

		if (NewSlot == nullptr)
		{
			break;
		}

		if (InitSlotBySkill(SkillCDO, NewSlot))
		{
			SkillVerticalBox->AddChild(NewSlot);
		}
	}
}

bool USkillUI::InitSlotBySkill(URaidBossSkillBase* SkillCDO, USkillSlot* NewSlot) const
{
	if (SkillCDO && NewSlot)
	{
		NewSlot->InitSlot(SkillCDO->GetAbilityTriggerTag(), SkillCDO->GetSkillInfo().SkillTexture, SkillCDO->GetSkillInfo().SkillName,
						  SkillCDO->GetSkillInfo().RequirePoint, SkillCDO->GetSkillInfo().SkillLevel);

		return true;
	}

	return false;
}
