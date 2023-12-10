// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "InteractionalUIAction.generated.h"

USTRUCT(BlueprintType)
struct FUIActionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Action Data")
	UObject*	Object1;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class RAIDBOSS_API UInteractionalUIAction : public UObject
{
	GENERATED_BODY()

public:
	UInteractionalUIAction(const FObjectInitializer & ObjectInitializer);
	
	virtual int32	ActivateUIAction(FUIActionData ActionData) const;

	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | UI Action")
	void	OnActionActivated(FUIActionData ActionData) const;

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	FGameplayTag	GetActionTriggerTag() const { return ActionTriggerTag; }
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss | UI Action")
	FGameplayTag	ActionTriggerTag;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
