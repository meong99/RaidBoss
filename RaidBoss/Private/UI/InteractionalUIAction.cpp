// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionalUIAction.h"

UInteractionalUIAction::UInteractionalUIAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 UInteractionalUIAction::ActivateUIAction(FUIActionData ActionData) const
{
    OnActionActivated(ActionData);

    return 0;
}
