#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Management/RaidBossGameplayTags.h"

URaidBossCharacterStatusAttributeSet::URaidBossCharacterStatusAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Mana(0.f)
	, MaxMana(0.f)
	, AttackPower(1.0f)
	, AttackRange(1.0f)
	, DefensePower(1.0f)
	, AdditionalAttackPower(0.0f)
	, AdditionalDefencePower(0.0f)
	, MoveSpeed(1.0f)
	, AttackSpeed(1.0f)
	, AttackPowerIncreaseRate(1.0f)
	, DefencePowerIncreaseRate(1.0f)
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

void URaidBossCharacterStatusAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	ARaidBossCharacterBase*	OwnerCharacter = GetOwnerCharacter();
	
	if (Attribute == GetMoveSpeedAttribute() && OwnerCharacter)
	{
		GetOwnerCharacter()->GetCharacterMovement()->MaxWalkSpeed = NewValue;
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
	URaidBossAbilitySystemComponent* AbilitySystemComponent =
		Cast<URaidBossAbilitySystemComponent>(GetOwningAbilitySystemComponent());
	
	if (AbilitySystemComponent && GetHealth() <= 0)
	{
		FGameplayEventData Payload;

		AbilitySystemComponent->HandleGameplayEvent(RaidBossGameplayTags::Get().Character_Death, &Payload);
	}
}
