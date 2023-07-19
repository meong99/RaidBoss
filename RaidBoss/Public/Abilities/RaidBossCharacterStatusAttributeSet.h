#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAttributeBase.h"
#include "AbilitySystemComponent.h"
#include "RaidBossCharacterStatusAttributeSet.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossCharacterStatusAttributeSet : public URaidBossAttributeBase
{
	GENERATED_BODY()

public:
	URaidBossCharacterStatusAttributeSet();
/*
 *	----------- Overrided
 */
public:
	virtual void	PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool	PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void	PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

/*
 *	----------- Binded by Delegate
 */
/*
 *	----------- Other Method
 */
protected:
	void	AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
										float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	void	CheckHealthAndToDeath() const;
	void	DisplayExecutedDamage(const FGameplayEffectModCallbackData& Data) const;
	
/*
 *	----------- Access
 */
public:
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AttackPower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AttackRange)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, DefensePower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AdditialnalAttackPower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MoveSpeed)
	
/*
 *	----------- Member Variables
 */
public:
	UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
		FGameplayAttributeData	Health;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
		FGameplayAttributeData	MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
		FGameplayAttributeData	Mana;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
		FGameplayAttributeData	MaxMana;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData	AttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData	AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData	DefensePower;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData	AdditialnalAttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
		FGameplayAttributeData	MoveSpeed;
};
