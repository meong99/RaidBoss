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

	virtual void	PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool	PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void	PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AttackPower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AttackRange)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, DefensePower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AdditionalAttackPower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, AdditionalDefencePower)
	ATTRIBUTE_ACCESSORS(URaidBossCharacterStatusAttributeSet, MoveSpeed)
	
protected:
	void	AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
										float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	void	CheckHealthAndToDeath() const;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	Health;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	MaxMana;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	AttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	DefensePower;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	AdditionalAttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	AdditionalDefencePower;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Stat Attribute", meta=(AllowPrivateAccess))
	FGameplayAttributeData	MoveSpeed;
};
