#pragma once

#include "CoreMinimal.h"
#include "RaidBossItemBase.h"
#include "RaidBossConsumableItem.generated.h"

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossConsumableItem : public URaidBossItemBase
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
		
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	Health = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	MaxHealth = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
    float	Mana = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	MaxMana = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AttackPower = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	DefensePower = 0;

	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
};
