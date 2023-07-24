#pragma once

#include "CoreMinimal.h"
#include "RaidBossItemBase.h"
#include "RaidBossConsumableItem.generated.h"

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossConsumableItem : public URaidBossItemBase
{
	GENERATED_BODY()
/*
*	----------- Overrided
*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
/*
*	----------- Binded by Delegate
*/

/*
*	----------- Other Method
*/
private:
	FGameplayEffectSpecHandle	CreateEffectSpecHandle();
	UGameplayEffect*			CraeteNewEffectObject();

/*
*	----------- Access
*/
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Consumable Item")
	TArray<FGameplayEffectExecutionDefinition> ItemExecutions;
};
