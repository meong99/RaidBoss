#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

bool URaidBossConsumableItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	bool bHasItem = OwnerCharacter ? OwnerCharacter->GetItemAmount(GetAbilityTriggerTag()) > 0 : false;
	
	return (bCanActivate && bHasItem);
}

void URaidBossConsumableItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec*		AbilitySpec;

	AbilitySpec = GetCurrentAbilitySpec();
	FGameplayEffectSpecHandle EffectSpecHandle = CreateEffectSpecHandle();
	
	if (AbilitySpec && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	if (OwnerCharacter)
	{
		OwnerCharacter->DecreaseOrRemoveInventoryData(GetAbilityTriggerTag());
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
