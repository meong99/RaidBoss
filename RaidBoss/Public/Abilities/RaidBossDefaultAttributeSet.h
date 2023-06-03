#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RaidBossDefaultAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class RAIDBOSS_API URaidBossDefaultAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URaidBossDefaultAttributeSet();

	// 어트리뷰트에 대한 수정 직전 호출되는 함수입니다. 어트리뷰트의 값에 대한 규칙을 적용하기 위한 것이며, 
	// 예로 "Health 값은 0 과 MaxHealth 사이여야 한다"거나, 어트리뷰트 변화에 게임내 반응을 발동하지 않는다던가 식입니다.
	virtual void	PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// 어트리뷰트의 값 수정 직전, 이 함수가 제안된 수정을 거부하거나 변경할 수 있습니다.
	virtual bool	PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	// 어트리뷰트의 값 수정 직후, 이 함수가 변화에 반응할 수 있습니다. 보통 어트리뷰트 최종 값에 대한 클램핑(범위 제한)이나 
	// 새 값에 대한 게임내 반응, 즉 "Health" 어트리뷰트가 0 으로 떨어지면 사망 효과 발동 등이 포함됩니다.
	virtual void	PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void	GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, AttackPower)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, DefensePower)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, MoveSpeed)
	ATTRIBUTE_ACCESSORS(URaidBossDefaultAttributeSet, Damage)

protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
		virtual void	OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_Mana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_AttackPower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_DefensePower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
		virtual void	OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData	Health;
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData	MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData	Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData	MaxMana;
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
		FGameplayAttributeData	AttackPower;
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
		FGameplayAttributeData	DefensePower;
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData	MoveSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData	Damage;
};
