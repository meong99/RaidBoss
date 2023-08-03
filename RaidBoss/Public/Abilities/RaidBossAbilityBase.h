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

/*
 *	----------- Overrided
 */
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
/*
 *	----------- Binded by Delegate
*/
/*
 *	----------- Other Method
 */
public:
	bool	UseAbility();
	
protected:
	FGameplayEffectSpecHandle					CreateEffectSpecHandle();
	UGameplayEffect*							CraeteNewEffect();

	URaidBossAbilitySystemComponent*			GetOwnerAbilityComponent() const;
	const URaidBossCharacterStatusAttributeSet*	GetOwnerCharacterState();
	FGameplayEffectSpecHandle					SetCallerMagnitudeByDataTag(TSubclassOf<UGameplayEffect> Effect,
													const FGameplayTag& DataTag, float Magnitude, float Level);
/*
 *	----------- Access
 */

/*
 *	----------- Member Variables
 */
public:
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
