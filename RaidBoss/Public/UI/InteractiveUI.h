// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractiveUIAction.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveUI.generated.h"

class UInteractiveUIAction;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UInteractiveUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Raid Boss | UI")
    void    SendUIActionEventToController(FGameplayTag ActionTriggerTag, FUIActionData ActionData) const;

    UFUNCTION(BlueprintCallable, Category="Raid Boss | UI")
    void    SendUIEventToController() const;

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    FGameplayTag                            GetUITriggerTag() const { return UITriggerTag; }
    FGameplayTagContainer                   GetUIRemoveTags() const { return UIRemoveTags; }
    const TArray<UInteractiveUIAction*>&    GetInteractiveUIActionArray() const { return InteractiveUIActionArray; }

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //UI를 켜고 끄는 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | UI")
    FGameplayTag                    UITriggerTag;

    //이 UI가 켜지면 이 태그를 갖고있는 UI는 지워짐
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | UI")
    FGameplayTagContainer           UIRemoveTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Raid Boss | UI")
    TArray<UInteractiveUIAction*>   InteractiveUIActionArray;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
