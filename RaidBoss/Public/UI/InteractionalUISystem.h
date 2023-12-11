// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractionalUIAction.h"
#include "Components/ActorComponent.h"
#include "InteractionalUISystem.generated.h"

class UInteractionalUIAction;
class UInteractionalUI;

USTRUCT(BlueprintType)
struct FInteractionalUIHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractionalUI*> InteractionalUI;
};

USTRUCT(BlueprintType)
struct FInteractionalUIActionHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractionalUIAction*> InteractionalUIActions;
};

USTRUCT(BlueprintType)
struct FRemoveUIHandle
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<UInteractionalUI*> InteractionalUI;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API UInteractionalUISystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionalUISystem();

public:
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    int32   HandleUITriggerEvent(FGameplayTag TriggerTag) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    int32   HandleUIActionTriggerEvent(FGameplayTag TriggerTag, FUIActionData ActionData) const;

    void    InteractionalUIParsing(const TArray<TSubclassOf<UInteractionalUI>>& InteractionalUIArray);

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Raid Boss | UI System")
    FInteractionalUIHandle  GetInteractionalUIHandle(FGameplayTag TriggerTag) const
    {
        return InteractionalUIMap.FindRef(TriggerTag);
    }

protected:
    void    AddRemovalTagKey(UInteractionalUI* InteractionalUI);

    void    InteractionalUIActionParsing(const TArray<UInteractionalUIAction*>& InteractionalUIActionArray);

    void    FindAndAddRemovalUI();

    void    RemoveOtherUIWithRemoveTag(FGameplayTagContainer RemoveTags) const;

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    TMap<FGameplayTag, FInteractionalUIHandle>          InteractionalUIMap;

    UPROPERTY()
    TMap<FGameplayTag, FInteractionalUIActionHandle>    InteractionalUIActionMap;

    UPROPERTY()
    TMap<FGameplayTag, FRemoveUIHandle>                 RemovalUIByTagMap;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
