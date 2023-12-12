#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "GameFramework/PlayerController.h"
#include "RaidBossPlayerControllerBase.generated.h"

class UInteractiveUISystem;
class UInteractiveUI;
class URaidBossRewardSystem;
class ARaidBossPlayerBase;
class URaidBossInventorySystem;
class URaidBossSkillSystem;
class URaidBossAbilityBase;
class URaidBossAbilitySystemComponent;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerControllerBase : public APlayerController
{
    GENERATED_BODY()

public:
    ARaidBossPlayerControllerBase();

protected:
    virtual void OnPossess(APawn* InPawn) override;

public:
    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    void MoveCharacter(FVector2D Value) const;

    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    void JumpCharacter() const;

    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    void StopJumpCharacter() const;

    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    void LookCharacter(FVector2D Value) const;

    UFUNCTION(BlueprintCallable, Category="Raid Boss | UI")
    void SendUIEventTagToController(FGameplayTag TriggerTag) const;

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    URaidBossAbilitySystemComponent*                GetRaidBossAbilitySystemComponent() const;
    UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
    ARaidBossPlayerBase*                            GetRaidBossPlayerBase() const;
    UFUNCTION(BlueprintCallable, Category="Raid Boss | UI")
    UInteractiveUISystem*                         GetInteractionalUISystem() const { return InteractionalUISystem; }
    const TArray<TSubclassOf<UInteractiveUI>>&    GetInteractionalUIArray() const { return InteractionalUIArray; }

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditDefaultsOnly, Category="Raid Boss | UI")
    TObjectPtr<UInteractiveUISystem>      InteractionalUISystem;

    UPROPERTY(EditDefaultsOnly, Category="Raid Boss | UI")
    TArray<TSubclassOf<UInteractiveUI>>   InteractionalUIArray;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
