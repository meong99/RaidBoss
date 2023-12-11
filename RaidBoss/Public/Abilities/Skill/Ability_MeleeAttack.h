    // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaidBossSkillBase.h"
#include "Ability_MeleeAttack.generated.h"

#define ComboInitValue 0

class UAbilityTask_WaitGameplayEvent;
class AWeapon;

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_MeleeAttack : public URaidBossSkillBase
{
    GENERATED_BODY()

public:
    virtual bool    CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                       FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual void    OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
    virtual void    ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData) override;

    virtual void    EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                               bool bWasCancelled) override;

public:
    virtual float   GetSkillRange() const override;

    UFUNCTION(BlueprintImplementableEvent)
    void    OnHitTarget(AActor* Target);

protected:
    virtual void    SetIndicator() override;

    UFUNCTION()
    void    NotifyMontageCanceledCallBack();

    UFUNCTION()
    void    NotifyAttackPointComesCallBack(FGameplayEventData Payload);

    UFUNCTION()
    void    NotifyComboResetCallBack(FGameplayEventData Payload);

    UFUNCTION()
    void    NotifyCanActivateNextAttackCallBack(FGameplayEventData Payload);

    void    ApplyEffectToTarget(const TArray<AActor*>& TargetActors);

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | MeleeAttack")
    int32   MaximumCombo = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | MeleeAttack")
    bool    bDrawDebugLine = false;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    TArray<UAbilityTask*>   SpawnedIndicatorTasks;

    UPROPERTY(BlueprintReadOnly, Category="Raid Boss | MeleeAttack")
    TObjectPtr<AWeapon>     CurrentWeapon;

    int32   CurrentCombo = ComboInitValue;

    bool    bCanActivateNextAttack = true;
};
