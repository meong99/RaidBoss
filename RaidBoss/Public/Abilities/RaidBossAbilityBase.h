#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/AbilityType.h"
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
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetAbilityTags();
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetBlockAbilityTags();
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Ability base")
	bool	CanActivateAbilityForBP() const;
	
	bool	UseAbility() const;
	
	ECharacterAbilityInputs						GetAbilityInputID() const;
	FGameplayTagContainer						GetAbilityInputTags() const { return AbilityInputTags; }
	FGameplayTag								GetAbilityTriggerTag() const;
	
protected:
	FGameplayEffectSpecHandle					CreateEffectSpecHandle();
	UGameplayEffect*							CreateNewEffect();

	URaidBossAbilitySystemComponent*			GetOwnerAbilityComponent() const;
	const URaidBossCharacterStatusAttributeSet*	GetOwnerCharacterState() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARaidBossCharacterBase>			OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	FGameplayTagContainer						AbilityInputTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	ECharacterAbilityInputs						AbilityInputID = ECharacterAbilityInputs::None;
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
