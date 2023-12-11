// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Aurora_E.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Aurora_E : public URaidBossSkillBase
{
    GENERATED_BODY()

public:
    UAbility_Aurora_E();

    virtual void    ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData) override;
    
    virtual void    CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateCancelAbility) override;   
protected:
    virtual void    EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                               bool bWasCancelled) override;

    virtual void    SetIndicator() override;

    UFUNCTION()
    void    AttackPointEventCallback(FGameplayEventData EventData);

    UFUNCTION()
    void    ToEndAbilityCallback();

    void    SpawnParticles() const;

    void    AttackTargetsInRadius() const;

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora E")
    TObjectPtr<UParticleSystem> Particle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Aurora E")
    float                       ParticleCount = 30;
    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    bool    bIsFirstTrigger = true;
};
