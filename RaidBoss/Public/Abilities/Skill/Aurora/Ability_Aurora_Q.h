// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Ability_Aurora_Q.generated.h"

class UDataPassingObject;
class UAT_DashCharacter;
class AWeapon;

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAbility_Aurora_Q : public URaidBossSkillBase
{
    GENERATED_BODY()

public:
    UAbility_Aurora_Q();

    virtual bool    CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                       FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
    virtual void    OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void    ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void    EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                               bool bWasCancelled) override;

    UFUNCTION()
    void    NotifyMontageEndedCallBack();

    UFUNCTION()
    void    NotifyAttackPointEventCallBack(FGameplayEventData Payload);

    UFUNCTION()
    void    NotifyBeginOverlappedCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult);

    UDataPassingObject* GetKnockBackData(AActor* TargetActor);

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill")
    float DashSpeed = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill")
    float DashDistance = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill")
    float KnockBackPower = 1000;


    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    UAT_DashCharacter*  DashCharacterTask = nullptr;

    UPROPERTY()
    TArray<AActor*>     TargetActors;

    TWeakObjectPtr<AWeapon> CurrentWeapon;

    bool                    bIsFirstActivation = true;
};
