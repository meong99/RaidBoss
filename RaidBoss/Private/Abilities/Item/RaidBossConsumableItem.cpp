#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

URaidBossConsumableItem::URaidBossConsumableItem()
{
}

void URaidBossConsumableItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		for (auto Element : Effects)
		{
			UGameplayEffect*	Effect = Element.GetDefaultObject();
			ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Effect, 1);
		}
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

//
// void URaidBossConsumableItem::UseItem()
// {
// 	URaidBossAbilitySystemComponent* AbilityComponent = Character ? Character->GetRaidBossAbilitySystemComponent() : nullptr;
//
// 	if (AbilityComponent)
// 	{
// 		FGameplayEffectSpecHandle SpecHandle = AddAllMagnitudeOfEffect(AbilityComponent);
// 		AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
// 	}
// }
//
// void URaidBossConsumableItem::UnuseItem()
// {
// 	URaidBossAbilitySystemComponent* AbilityComponent = Character ? Character->GetRaidBossAbilitySystemComponent() : nullptr;
//
// 	if (AbilityComponent)
// 	{
// 		FGameplayEffectSpecHandle SpecHandle = SubAllMagnitudeOfEffect(AbilityComponent);
// 		AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
// 	}
// }
