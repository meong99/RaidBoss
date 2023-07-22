#include "Management/RaidBossSkillSystem.h"

#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/RaidBossSkillWidget.h"

void URaidBossSkillSystem::InitialzeSkillSystem(URaidBossSkillWidget* InSkillWidget)
{
	GiveSkillToAbilityComponent();
	if (InSkillWidget)
	{
		SkillWidget = InSkillWidget;
		SkillWidget->InitializeSkillWidget(this);
	}
}

void URaidBossSkillSystem::ToggleSkillWidget()
{
	ARaidBossPlayerControllerBase* PlayerController;

	PlayerController = GetRaidBossPlayerControllerBase();
	
	if (PlayerController && SkillWidget && SkillWidget->IsInViewport() == false)
	{
		FInputModeGameAndUI InputMode;

		InputMode.SetWidgetToFocus(SkillWidget->TakeWidget());
		SkillWidget->AddToViewport();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else if (PlayerController && SkillWidget)
	{
		FInputModeGameOnly InputMode;
		
		SkillWidget->RemoveFromParent();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

void URaidBossSkillSystem::GiveSkillToAbilityComponent()
{
	URaidBossSkillBase*						SkillCDO;
	FGameplayAbilitySpecHandle				SpecHandle;
	
	if (AbilitySystemComponent)
	{
		for (auto SkillClass : SkillClasses)
		{
			SkillCDO = Cast<URaidBossSkillBase>(SkillClass->GetDefaultObject());
			AbilitySystemComponent->GiveAbilityWithoutDuplication(SkillClass, SpecHandle, static_cast<int32>(SkillCDO->AbilityInputID));
		}
	}
}

ARaidBossPlayerControllerBase* URaidBossSkillSystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

TArray<TSubclassOf<URaidBossSkillBase>> URaidBossSkillSystem::GetSkillClasses() const
{
	return SkillClasses;
}

const URaidBossSkillBase* URaidBossSkillSystem::GetSkillObject(int32 Index)
{
	URaidBossSkillBase* SkillObject = nullptr;
	
	if (SkillClasses.IsValidIndex(Index))
		SkillObject = Cast<URaidBossSkillBase>(AbilitySystemComponent->GetAbilityByClass(SkillClasses[Index]));

	return SkillObject;
}

int32 URaidBossSkillSystem::GetCurrentSkillPoint() const
{
	return CurrentSkillPoint;
}
