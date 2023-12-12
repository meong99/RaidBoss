// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractiveUIAction.h"
#include "Components/ActorComponent.h"
#include "InteractiveUISystem.generated.h"

class UInteractiveUIAction;
class UInteractiveUI;

USTRUCT(BlueprintType)
struct FInteractiveUIHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractiveUI*> InteractiveUI;
};

USTRUCT(BlueprintType)
struct FInteractiveUIActionHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractiveUIAction*> InteractiveUIActions;
};

USTRUCT(BlueprintType)
struct FRemoveUIHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractiveUI*> InteractiveUI;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API UInteractiveUISystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractiveUISystem();

public:
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    int32   HandleUITriggerEvent(FGameplayTag TriggerTag) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    int32   HandleUIActionTriggerEvent(FGameplayTag TriggerTag, FUIActionData ActionData) const;

    void    InteractiveUIParsing(const TArray<TSubclassOf<UInteractiveUI>>& InteractionalUIArray);

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    FInteractiveUIHandle  GetInteractiveUIHandle(FGameplayTag TriggerTag) const
    {
        return InteractiveUIMap.FindRef(TriggerTag);
    }

protected:
    void    AddRemovalTagKey(UInteractiveUI* InteractionalUI);

    void    InteractiveUIActionParsing(const TArray<UInteractiveUIAction*>& InteractionalUIActionArray);

    void    FindAndAddRemovalUI();

    void    RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const;

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    TMap<FGameplayTag, FInteractiveUIHandle>        InteractiveUIMap;

    UPROPERTY()
    TMap<FGameplayTag, FInteractiveUIActionHandle>  InteractiveUIActionMap;

    UPROPERTY()
    TMap<FGameplayTag, FRemoveUIHandle>             RemoveUIByTagMap;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
