#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Management/RaidBossGameplayTags.h"

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

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ItemEffect);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackPower, AttackPower);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_DefensePower, DefensePower);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxHealth, MaxHealth);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxMana, MaxMana);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Health, Health);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Mana, Mana);
	
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	
	if (AbilitySpec && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	
		if (OwnerCharacter)
		{
			OwnerCharacter->DecreaseOrRemoveInventoryData(GetAbilityTriggerTag());
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
