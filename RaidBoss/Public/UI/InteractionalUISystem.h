// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InteractionalUISystem.generated.h"


class UInteractionalUIAction;
class UInteractionalUI;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API UInteractionalUISystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionalUISystem();

	int32	HandleUITriggerEvent(FGameplayTag TriggerTag) const;
	int32	HandleUIActionTriggerEvent(FGameplayTag TriggerTag) const;
	
	void	TriggerTagParsing();
protected:
	virtual void BeginPlay() override;
	
	void	InteractionalUIParsing(const TArray<TSubclassOf<UInteractionalUI>>& InteractionalUIArray);

	void	AddRemovalTagKey(UInteractionalUI* InteractionalUI);
	
	void	InteractionalUIActionParsing(const TArray<UInteractionalUIAction*>& InteractionalUIActionArray);

	void	FindAndAddRemovalUI();
	
	void	RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const;
	
	const TMap<FGameplayTag, TArray<UInteractionalUI*>>&		GetInteractionalUIes() const { return InteractionalUIes; }
	const TMap<FGameplayTag, TArray<UInteractionalUIAction*>>&	GetInteractionalUIActions() const { return InteractionalUIActions; };
	
protected:
	TMap<FGameplayTag, TArray<UInteractionalUI*>>		InteractionalUIes;
	TMap<FGameplayTag, TArray<UInteractionalUIAction*>> InteractionalUIActions;
	TMap<FGameplayTag, TArray<UInteractionalUI*>>		RemovalUIByTag;
};
