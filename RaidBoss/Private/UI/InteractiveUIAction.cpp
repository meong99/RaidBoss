// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractiveUIAction.h"

UInteractiveUIAction::UInteractiveUIAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 UInteractiveUIAction::ActivateUIAction(FUIActionData ActionData) const
{
    OnActionActivated(ActionData);

    return 0;
}
