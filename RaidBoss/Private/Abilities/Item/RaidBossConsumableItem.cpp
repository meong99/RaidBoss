#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

void URaidBossConsumableItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec*	Spec;
	UGameplayEffect*		EffectToApply;
	
	Spec = GetCurrentAbilitySpec();
	EffectToApply = ItemEffect.GetDefaultObject();
	
	if (Spec && EffectToApply && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		EffectToApply->Executions = ItemExecutions;
	
		Spec->GameplayEffectHandle = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectToApply, 1);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool URaidBossConsumableItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return (bCanActivate);
}
