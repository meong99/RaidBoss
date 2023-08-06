#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Global/RaidBoss.h"
#include "RaidBossAbilityBase.generated.h"

class URaidBossCharacterStatusAttributeSet;
class URaidBossAbilitySystemComponent;
class ARaidBossCharacterBase;

UCLASS(Abstract)
class RAIDBOSS_API URaidBossAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URaidBossAbilityBase();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	bool	UseAbility() const;
	
	ERaidBossAbilityInputID						GetAbilityInputID() const;
	
protected:
	FGameplayEffectSpecHandle					CreateEffectSpecHandle();
	UGameplayEffect*							CreateNewEffect();

	URaidBossAbilitySystemComponent*			GetOwnerAbilityComponent() const;
	const URaidBossCharacterStatusAttributeSet*	GetOwnerCharacterState() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARaidBossCharacterBase>			OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	ERaidBossAbilityInputID						AbilityInputID = ERaidBossAbilityInputID::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	TObjectPtr<UTexture2D>						AbilityTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	TSubclassOf<UGameplayEffect>				EffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	TArray<TObjectPtr<UAnimMontage>>			Montages;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Ability base")
	TArray<FGameplayEffectExecutionDefinition>	EffectExecutions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Ability base")
	TArray<FGameplayModifierInfo>				EffectModifiers;
};
