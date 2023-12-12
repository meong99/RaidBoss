// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InteractiveUISystem.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/InteractiveUI.h"
#include "UI/InteractiveUIAction.h"

UInteractiveUISystem::UInteractiveUISystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

int32 UInteractiveUISystem::HandleUITriggerEvent(FGameplayTag TriggerTag) const
{
    const FInteractiveUIHandle& InteractionalUIHandle = InteractiveUIMap.FindRef(TriggerTag);

    if (InteractionalUIHandle.InteractionalUI.IsEmpty())
    {
        return 0;
    }

    int32 AddedWidgets = 0;

    for (int i = 0; i < InteractionalUIHandle.InteractionalUI.Num(); i++)
    {
        if (InteractionalUIHandle.InteractionalUI.IsValidIndex(i) == false)
        {
            continue;
        }

        if (InteractionalUIHandle.InteractionalUI[i]->IsInViewport())
        {
            InteractionalUIHandle.InteractionalUI[i]->RemoveFromParent();
        }
        else
        {
            RemoveOtherUIWithRemoveTag(InteractionalUIHandle.InteractionalUI[i]->GetUIRemoveTags());
            InteractionalUIHandle.InteractionalUI[i]->AddToViewport();

            AddedWidgets++;
        }
    }

    return AddedWidgets;
}

int32 UInteractiveUISystem::HandleUIActionTriggerEvent(FGameplayTag TriggerTag, FUIActionData ActionData) const
{
    const FInteractiveUIActionHandle& UIActionHandle = InteractiveUIActionMap.FindRef(TriggerTag);

    if (UIActionHandle.InteractionalUIActions.IsEmpty())
    {
        return 0;
    }

    int32 ActivatedActions = 0;

    for (const UInteractiveUIAction* InteractionalAction : UIActionHandle.InteractionalUIActions)
    {
        ActivatedActions += InteractionalAction->ActivateUIAction(ActionData);
    }

    return ActivatedActions;
}

void UInteractiveUISystem::InteractionalUIParsing(const TArray<TSubclassOf<UInteractiveUI>>& InteractionalUIArray)
{
    ARaidBossPlayerControllerBase* ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwner());

    if (ControllerBase == nullptr)
    {
        return;
    }

    for (const auto& InteractionalUIClass : InteractionalUIArray)
    {
        if (IsValid(InteractionalUIClass) == false)
        {
            continue;
        }

        UInteractiveUI* InteractionalUI = Cast<UInteractiveUI>(CreateWidget(ControllerBase, InteractionalUIClass));

        if (InteractionalUI && InteractionalUI->GetUITriggerTag().IsValid())
        {
            FInteractiveUIHandle& InteractionalUIHandle = InteractiveUIMap.FindOrAdd(
                InteractionalUI->GetUITriggerTag());
            
            InteractionalUIHandle.InteractionalUI.Add(InteractionalUI);
            AddRemovalTagKey(InteractionalUI);
            InteractionalUIActionParsing(InteractionalUI->GetInteractiveUIActionArray());
        }
    }

    FindAndAddRemovalUI();
}

void UInteractiveUISystem::AddRemovalTagKey(UInteractiveUI* InteractionalUI)
{
    const FGameplayTagContainer& RemoveTags = InteractionalUI->GetUIRemoveTags();

    for (auto RemoveTag : RemoveTags)
    {
        RemoveUIByTagMap.Add(RemoveTag);
    }
}

void UInteractiveUISystem::InteractionalUIActionParsing(
    const TArray<UInteractiveUIAction*>& InteractionalUIActionArray)
{
    for (UInteractiveUIAction* InteractionalUIAction : InteractionalUIActionArray)
    {
        if (InteractionalUIAction)
        {
            FInteractiveUIActionHandle& UIActionHandle = InteractiveUIActionMap.FindOrAdd(
                InteractionalUIAction->GetActionTriggerTag());
            
            UIActionHandle.InteractionalUIActions.Add(InteractionalUIAction);
        }
    }
}

void UInteractiveUISystem::FindAndAddRemovalUI()
{
    TSet<FGameplayTag> OutRemoveTags;

    RemoveUIByTagMap.GetKeys(OutRemoveTags);

    for (const FGameplayTag& RemoveTag : OutRemoveTags)
    {
        FRemoveUIHandle* RemoveUIHandle = RemoveUIByTagMap.Find(RemoveTag);

        if (RemoveUIHandle)
        {
            FInteractiveUIHandle* InteractionalUIHandle = InteractiveUIMap.Find(RemoveTag);

            if (InteractionalUIHandle == nullptr)
            {
                break;
            }

            for (UInteractiveUI* Ui : InteractionalUIHandle->InteractionalUI)
            {
                RemoveUIHandle->InteractionalUI.Add(Ui);
            }
        }
    }
}

void UInteractiveUISystem::RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const
{
    for (auto RemoveTag : RemoveTags)
    {
        const FRemoveUIHandle* RemoveUIHandle = RemoveUIByTagMap.Find(RemoveTag);

        if (RemoveUIHandle == nullptr)
        {
            return;
        }

        for (UInteractiveUI* InteractionalUI : RemoveUIHandle->InteractionalUI)
        {
            if (InteractionalUI)
            {
                InteractionalUI->RemoveFromParent();
            }
        }
    }
}
