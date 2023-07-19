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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	ERaidBossAbilityInputID					AbilityInputID = ERaidBossAbilityInputID::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D>					AbilityTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TArray<TSubclassOf<UGameplayEffect>>	Effects;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARaidBossCharacterBase>		OwnerCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>>		Montages;
};
