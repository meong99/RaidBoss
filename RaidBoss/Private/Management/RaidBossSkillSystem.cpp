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
		SkillWidget->AddToViewportWithTracking();
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else if (PlayerController && SkillWidget)
	{
		FInputModeGameOnly	InputMode;
		int32				RemainingWidgets = 0;
		
		SkillWidget->RemoveWidget(SkillWidget, RemainingWidgets);
		if (RemainingWidgets == 0)
		{
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void URaidBossSkillSystem::UseSkill(int32 Index)
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();

	if (AbilitySystemComponent && SkillClasses.IsValidIndex(Index))
	{
		const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(SkillClasses[Index]);
		
		AbilitySystemComponent->TryActivateAbility(AbilitySpec->Handle);
	}
}

int32 URaidBossSkillSystem::IncreaseSkillLevel(int32 Index)
{
	URaidBossSkillBase* SkillCDO = nullptr;
	if (SkillClasses.IsValidIndex(Index))
		SkillCDO = SkillClasses[Index].GetDefaultObject();
	
	URaidBossSkillBase*	SkillInstance = GetSkillInstance(Index);
	
	if (SkillCDO && SkillInstance && CurrentSkillPoint > 0)
	{
		SkillCDO->IncreaseSkillLevel();
		if (SkillInstance->IncreaseSkillLevel())
			CurrentSkillPoint--;
	}

	int32 CurrentSkillLevel = SkillInstance->GetSkillInfo().SkillLevel;
	
	return CurrentSkillLevel;
}

int32 URaidBossSkillSystem::DecreaseSkillLevel(int32 Index)
{
	URaidBossSkillBase* SkillCDO = nullptr;
	
	if (SkillClasses.IsValidIndex(Index))
		SkillCDO = SkillClasses[Index].GetDefaultObject();
	
	URaidBossSkillBase*	SkillInstance = GetSkillInstance(Index);

	if (SkillCDO && SkillInstance && SkillInstance->GetSkillInfo().SkillLevel > 0)
	{
		SkillCDO->DecreaseSkillLevel();
		if (SkillInstance->DecreaseSkillLevel())
			CurrentSkillPoint++;
	}
	
	int32 CurrentSkillLevel = SkillInstance->GetSkillInfo().SkillLevel;

	return CurrentSkillLevel;
}

void URaidBossSkillSystem::GiveSkillToAbilityComponent()
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	
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

URaidBossAbilitySystemComponent* URaidBossSkillSystem::GetRaidBossAbilitySystemComponent() const
{
	ARaidBossPlayerControllerBase*		Controller = GetRaidBossPlayerControllerBase();
	URaidBossAbilitySystemComponent*	AbilitySystemComponent = Controller ? Controller->GetRaidBossAbilitySystemComponent() : nullptr;

	return AbilitySystemComponent;
}

URaidBossSkillBase* URaidBossSkillSystem::GetSkillInstance(int32 Index) const
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();

	if (AbilitySystemComponent)
		return Cast<URaidBossSkillBase>(AbilitySystemComponent->GetAbilityByClass(SkillClasses[Index]));

	return nullptr;
}

const URaidBossSkillBase* URaidBossSkillSystem::GetSkillCDO(int32 Index) const
{
	if (SkillClasses.IsValidIndex(Index))
		return SkillClasses[Index].GetDefaultObject();

	return nullptr;
}

int32 URaidBossSkillSystem::GetSkillCount() const
{
	return  SkillClasses.Num();
}

int32 URaidBossSkillSystem::GetCurrentSkillPoint() const
{
	return CurrentSkillPoint;
}
