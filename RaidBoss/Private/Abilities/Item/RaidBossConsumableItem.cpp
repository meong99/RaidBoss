#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
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

	if (CanApplyEffectToOwner())
	{
		ApplyEffectToOwner();
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URaidBossConsumableItem::ApplyEffectToOwner() const
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ItemEffect);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackPower, AttackPower);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_DefensePower, DefensePower);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxHealth, MaxHealth);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxMana, MaxMana);
	
	bool	IsItOverHealth = Health + OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth()
							 > OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth();
	float	AdjustedHealth = Health;
	
	if (IsItOverHealth)
	{
		AdjustedHealth = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth()
						 - OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth();
	}
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Health, AdjustedHealth);

	bool	IsItOverMana = Mana + OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana()
						   > OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana();
	float	AdjustedMana = Mana;
	
	if (IsItOverMana)
	{
		AdjustedMana = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana()
					   - OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana();
	}
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Mana, AdjustedMana);
	
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	
	if (AbilitySpec)
	{
		AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	
		if (OwnerCharacter)
		{
			OwnerCharacter->DecreaseOrRemoveInventoryData(GetAbilityTriggerTag());
		}
	}
}

bool URaidBossConsumableItem::CanApplyEffectToOwner() const
{
	if (OwnerCharacter == nullptr)
	{
		return false;
	}

	bool	IsItFullHP = OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth()
						 == OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth();

	bool	IsItFullMana = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana()
						 == OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana();
	
	if ((IsItFullHP && Health > 0) || (IsItFullMana && Mana > 0))
	{
		return false;
	}

	return true;
}
