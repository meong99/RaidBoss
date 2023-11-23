// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InteractionalUISystem.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/InteractionalUI.h"
#include "UI/InteractionalUIAction.h"

UInteractionalUISystem::UInteractionalUISystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UInteractionalUISystem::HandleUITriggerEvent(FGameplayTag TriggerTag) const
{
	const auto& UIArray = InteractionalUIes.FindRef(TriggerTag);

	if (UIArray.IsEmpty())
	{
		return 0;
	}

	int32 AddedWidgets = 0;
	
	for (const auto InteractionalUI : UIArray)
	{
		if (InteractionalUI->IsInViewport())
		{
			InteractionalUI->RemoveFromParent();
		}
		else
		{
			RemoveOtherUIWithRemoveTag(InteractionalUI->GetUIRemoveTags());
			InteractionalUI->AddToViewport();

			AddedWidgets++;
		}
	}

	return AddedWidgets;
}

int32 UInteractionalUISystem::HandleUIActionTriggerEvent(FGameplayTag TriggerTag) const
{
	const auto& UIActionArray = InteractionalUIActions.FindRef(TriggerTag);

	if (UIActionArray.IsEmpty())
	{
		return 0;
	}

	int32 ActivatedActions = 0;
	
	for (const auto InteractionalAction : UIActionArray)
	{
		ActivatedActions += InteractionalAction->ActivateUIAction();
	}

	return ActivatedActions;
}

void UInteractionalUISystem::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionalUISystem::TriggerTagParsing()
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwner());

	if (ControllerBase)
	{
		InteractionalUIParsing(ControllerBase->GetInteractionalUIArray());
	}
}

void UInteractionalUISystem::InteractionalUIParsing(const TArray<TSubclassOf<UInteractionalUI>>& InteractionalUIArray)
{
	ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwner());

	if (ControllerBase == nullptr)
	{
		return;
	}
	
	for (const auto& InteractionalUIClass : InteractionalUIArray)
	{
		UInteractionalUI*	InteractionalUI = Cast<UInteractionalUI>(CreateWidget(ControllerBase, InteractionalUIClass));

		if (InteractionalUI && InteractionalUI->GetUITriggerTag().IsValid())
		{
			auto& UIArray = InteractionalUIes.FindOrAdd(InteractionalUI->GetUITriggerTag());
			UIArray.Add(InteractionalUI);

			AddRemovalTagKey(InteractionalUI);
			
			InteractionalUIActionParsing(InteractionalUI->GetInteractionalUIActionArray());
		}
	}
	
	FindAndAddRemovalUI();
}

void UInteractionalUISystem::AddRemovalTagKey(UInteractionalUI* InteractionalUI)
{
	const FGameplayTagContainer& RemoveTags = InteractionalUI->GetUIRemoveTags();

	for (auto RemoveTag : RemoveTags)
	{
		RemovalUIByTag.Add(RemoveTag);
	}

}

void UInteractionalUISystem::InteractionalUIActionParsing(const TArray<UInteractionalUIAction*>& InteractionalUIActionArray)
{
	for (const auto& InteractionalUIAction : InteractionalUIActionArray)
	{
		if (InteractionalUIAction)
		{
			auto& UIActionArray = InteractionalUIActions.FindOrAdd(InteractionalUIAction->GetActionTriggerTag());
			UIActionArray.Add(InteractionalUIAction);
		}
	}
}

void UInteractionalUISystem::FindAndAddRemovalUI()
{
	TSet<FGameplayTag> OutRemoveTags;
	
	RemovalUIByTag.GetKeys(OutRemoveTags);

	for (const auto& RemoveTag : OutRemoveTags)
	{
		if (auto RemovalUIArray = RemovalUIByTag.Find(RemoveTag))
		{
			TArray<UInteractionalUI*>* UIesByTag = InteractionalUIes.Find(RemoveTag);

			if (UIesByTag == nullptr)
			{
				break;
			}
			
			for (auto Ui : *UIesByTag)
			{
				RemovalUIArray->Add(Ui);
			}
		}
	}
}

void UInteractionalUISystem::RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const
{
	for (auto RemoveTag : RemoveTags)
	{
		const TArray<UInteractionalUI*>* UIArray = RemovalUIByTag.Find(RemoveTag);
	
		if (UIArray == nullptr)
		{
			return;
		}
		
		for (const auto& UI : *UIArray)
		{
			if (UI)
			{
				UI->RemoveFromParent();
			}
		}
	}
}
