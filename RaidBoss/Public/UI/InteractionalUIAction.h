// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "InteractionalUIAction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class RAIDBOSS_API UInteractionalUIAction : public UObject
{
	GENERATED_BODY()

public:
	UInteractionalUIAction(const FObjectInitializer & ObjectInitializer);
	virtual int32	ActivateUIAction();

	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | UI Action")
	void OnActionActivated();
	
	FGameplayTag	GetActionTriggerTag() const { return ActionTriggerTag; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss | UI Action")
	FGameplayTag	ActionTriggerTag;
};
