// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionalUI.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "UI/InteractionalUISystem.h"

void UInteractionalUI::SendUIActionEventToController(FGameplayTag ActionTriggerTag) const
{
	const ARaidBossPlayerControllerBase*	ControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());
	const UInteractionalUISystem*			InteractionalUISystem = ControllerBase ? ControllerBase->GetInteractionalUISystem() : nullptr;
	
	if (InteractionalUISystem)
	{
		int32 ActivatedActions = InteractionalUISystem->HandleUIActionTriggerEvent(ActionTriggerTag);
	}
}
