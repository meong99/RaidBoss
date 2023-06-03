#include "Abilities/RaidBossDefaultAttributeSet.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Player/RaidBossPlayerBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RaidBoss/RaidBoss.h"

URaidBossDefaultAttributeSet::URaidBossDefaultAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, MoveSpeed(0.0f)
	, Damage(0.0f)
{
}

void URaidBossDefaultAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

bool URaidBossDefaultAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	Super::PreGameplayEffectExecute(Data);
	return true;
}

void URaidBossDefaultAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void URaidBossDefaultAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URaidBossDefaultAttributeSet, Health);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, MaxHealth);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, Mana);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, MaxMana);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, AttackPower);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, DefensePower);
	DOREPLIFETIME(URaidBossDefaultAttributeSet, MoveSpeed);
}

void URaidBossDefaultAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void URaidBossDefaultAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, Health, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, MaxHealth, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, Mana, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, MaxMana, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, AttackPower, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, DefensePower, OldValue);
}

void URaidBossDefaultAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URaidBossDefaultAttributeSet, MoveSpeed, OldValue);
}
