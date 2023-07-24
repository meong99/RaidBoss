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

void URaidBossSkillSystem::ToggleSkillWidget() const
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
	if (AbilitySystemComponent)
	{
		for (auto SkillClass : SkillClasses)
		{
			const URaidBossSkillBase*	SkillCDO = Cast<URaidBossSkillBase>(SkillClass->GetDefaultObject());
			FGameplayAbilitySpecHandle	OutSpecHandle;
			
			AbilitySystemComponent->GiveAbilityWithoutDuplication(SkillClass, OutSpecHandle, static_cast<int32>(SkillCDO->AbilityInputID));
		}
	}
}

ARaidBossPlayerControllerBase* URaidBossSkillSystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOuter());
}

const URaidBossSkillBase* URaidBossSkillSystem::GetSkillObject(int32 Index) const
{
	if (SkillClasses.IsValidIndex(Index))
		return Cast<URaidBossSkillBase>(AbilitySystemComponent->GetAbilityByClass(SkillClasses[Index]));

	return nullptr;
}

int32 URaidBossSkillSystem::GetSkillAmount() const
{
	return  SkillClasses.Num();
}

int32 URaidBossSkillSystem::GetCurrentSkillPoint() const
{
	return CurrentSkillPoint;
}
