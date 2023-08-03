#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Character/RaidBossCharacterBase.h"

URaidBossCharacterStatusAttributeSet::URaidBossCharacterStatusAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, AttackPower(1.0f)
	, AttackRange(0.0f)
	, DefensePower(1.0f)
	, AdditionalAttackPower(0.0f)
	, AdditionalDefencePower(0.0f)
	, MoveSpeed(0.0f)
{
}

void URaidBossCharacterStatusAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

bool URaidBossCharacterStatusAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	Super::PreGameplayEffectExecute(Data);

	return true;
}

void URaidBossCharacterStatusAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	CheckHealthAndToDeath();
}

void URaidBossCharacterStatusAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
                                                                       const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && IsValid(AbilityComp))
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void URaidBossCharacterStatusAttributeSet::CheckHealthAndToDeath() const
{
	ARaidBossCharacterBase* OwnerCharacter = GetOwnerCharacter();
	
	if (GetHealth() <= 0 && IsValid(OwnerCharacter) == true)
	{
		OwnerCharacter->OnDeath();
	}
}
