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

	// ��Ʈ����Ʈ�� ���� ���� ���� ȣ��Ǵ� �Լ��Դϴ�. ��Ʈ����Ʈ�� ���� ���� ��Ģ�� �����ϱ� ���� ���̸�, 
	// ���� "Health ���� 0 �� MaxHealth ���̿��� �Ѵ�"�ų�, ��Ʈ����Ʈ ��ȭ�� ���ӳ� ������ �ߵ����� �ʴ´ٴ��� ���Դϴ�.
	virtual void	PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// ��Ʈ����Ʈ�� �� ���� ����, �� �Լ��� ���ȵ� ������ �ź��ϰų� ������ �� �ֽ��ϴ�.
	virtual bool	PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	// ��Ʈ����Ʈ�� �� ���� ����, �� �Լ��� ��ȭ�� ������ �� �ֽ��ϴ�. ���� ��Ʈ����Ʈ ���� ���� ���� Ŭ����(���� ����)�̳� 
	// �� ���� ���� ���ӳ� ����, �� "Health" ��Ʈ����Ʈ�� 0 ���� �������� ��� ȿ�� �ߵ� ���� ���Ե˴ϴ�.
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
