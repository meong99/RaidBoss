// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InteractiveUI.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/InteractiveUISystem.h"

void UInteractiveUI::SendUIActionEventToController(FGameplayTag ActionTriggerTag, FUIActionData ActionData) const
{
    const ARaidBossPlayerControllerBase* ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());
    const UInteractiveUISystem* InteractionalUISystem = ControllerBase
                                                              ? ControllerBase->GetInteractionalUISystem()
                                                              : nullptr;

    if (InteractionalUISystem)
    {
        int32 ActivatedActions = InteractionalUISystem->HandleUIActionTriggerEvent(ActionTriggerTag, ActionData);
    }
}

void UInteractiveUI::SendUIEventToController() const
{
    const ARaidBossPlayerControllerBase* ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());
    const UInteractiveUISystem* InteractionalUISystem = ControllerBase
                                                              ? ControllerBase->GetInteractionalUISystem()
                                                              : nullptr;

    if (InteractionalUISystem)
    {
        int32 ActivatedActions = InteractionalUISystem->HandleUITriggerEvent(UITriggerTag);
    }
}
