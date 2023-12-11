#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Character/RaidBossCharacterBase.h"
#include "Management/RaidBossGameplayTags.h"

bool URaidBossConsumableItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags,
                                                 FGameplayTagContainer* OptionalRelevantTags) const
{
    bool    bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
    bool    bHasItem = OwnerCharacter ? OwnerCharacter->GetItemAmount(GetAbilityTriggerTag()) > 0 : false;

    return bCanActivate && bHasItem;
}

void URaidBossConsumableItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
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
    FGameplayEffectSpecHandle   EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ItemEffect);
    bool                        bIsItOverHealth = Health + OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth() >
                                                  OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth();
    bool                        bIsItOverMana = Mana + OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana() >
                                                OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana();
    float                       AdjustedHealth = Health;
    float                       AdjustedMana = Mana;

    if (bIsItOverHealth)
    {
        AdjustedHealth = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth() -
                         OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth();
    }

    if (bIsItOverMana)
    {
        AdjustedMana = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana() -
                       OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana();
    }
    
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Health, AdjustedHealth);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_Mana, AdjustedMana);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackPower, AttackPower);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_DefensePower,DefensePower);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxHealth, MaxHealth);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_MaxMana, MaxMana);

    FGameplayAbilitySpec*   AbilitySpec = GetCurrentAbilitySpec();

    if (AbilitySpec)
    {
        AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo,
                                                                           CurrentActivationInfo, EffectSpecHandle);

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

    bool bIsItFullHP = OwnerCharacter->GetCharacterStatusAttributeSet()->GetHealth() ==
                       OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxHealth();

    bool bIsItFullMana = OwnerCharacter->GetCharacterStatusAttributeSet()->GetMana() ==
                         OwnerCharacter->GetCharacterStatusAttributeSet()->GetMaxMana();

    if ((bIsItFullHP && Health > 0) || (bIsItFullMana && Mana > 0))
    {
        return false;
    }

    return true;
}
