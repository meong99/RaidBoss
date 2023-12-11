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
    const FInteractionalUIHandle& InteractionalUIHandle = InteractionalUIMap.FindRef(TriggerTag);

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

int32 UInteractionalUISystem::HandleUIActionTriggerEvent(FGameplayTag TriggerTag, FUIActionData ActionData) const
{
    const FInteractionalUIActionHandle& UIActionHandle = InteractionalUIActionMap.FindRef(TriggerTag);

    if (UIActionHandle.InteractionalUIActions.IsEmpty())
    {
        return 0;
    }

    int32 ActivatedActions = 0;

    for (const UInteractionalUIAction* InteractionalAction : UIActionHandle.InteractionalUIActions)
    {
        ActivatedActions += InteractionalAction->ActivateUIAction(ActionData);
    }

    return ActivatedActions;
}

void UInteractionalUISystem::InteractionalUIParsing(const TArray<TSubclassOf<UInteractionalUI>>& InteractionalUIArray)
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

        UInteractionalUI* InteractionalUI = Cast<UInteractionalUI>(CreateWidget(ControllerBase, InteractionalUIClass));

        if (InteractionalUI && InteractionalUI->GetUITriggerTag().IsValid())
        {
            FInteractionalUIHandle& InteractionalUIHandle = InteractionalUIMap.FindOrAdd(
                InteractionalUI->GetUITriggerTag());
            
            InteractionalUIHandle.InteractionalUI.Add(InteractionalUI);
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
        RemovalUIByTagMap.Add(RemoveTag);
    }
}

void UInteractionalUISystem::InteractionalUIActionParsing(
    const TArray<UInteractionalUIAction*>& InteractionalUIActionArray)
{
    for (UInteractionalUIAction* InteractionalUIAction : InteractionalUIActionArray)
    {
        if (InteractionalUIAction)
        {
            FInteractionalUIActionHandle& UIActionHandle = InteractionalUIActionMap.FindOrAdd(
                InteractionalUIAction->GetActionTriggerTag());
            
            UIActionHandle.InteractionalUIActions.Add(InteractionalUIAction);
        }
    }
}

void UInteractionalUISystem::FindAndAddRemovalUI()
{
    TSet<FGameplayTag> OutRemoveTags;

    RemovalUIByTagMap.GetKeys(OutRemoveTags);

    for (const FGameplayTag& RemoveTag : OutRemoveTags)
    {
        FRemoveUIHandle* RemoveUIHandle = RemovalUIByTagMap.Find(RemoveTag);

        if (RemoveUIHandle)
        {
            FInteractionalUIHandle* InteractionalUIHandle = InteractionalUIMap.Find(RemoveTag);

            if (InteractionalUIHandle == nullptr)
            {
                break;
            }

            for (UInteractionalUI* Ui : InteractionalUIHandle->InteractionalUI)
            {
                RemoveUIHandle->InteractionalUI.Add(Ui);
            }
        }
    }
}

void UInteractionalUISystem::RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const
{
    for (auto RemoveTag : RemoveTags)
    {
        const FRemoveUIHandle* RemoveUIHandle = RemovalUIByTagMap.Find(RemoveTag);

        if (RemoveUIHandle == nullptr)
        {
            return;
        }

        for (UInteractionalUI* InteractionalUI : RemoveUIHandle->InteractionalUI)
        {
            if (InteractionalUI)
            {
                InteractionalUI->RemoveFromParent();
            }
        }
    }
}
