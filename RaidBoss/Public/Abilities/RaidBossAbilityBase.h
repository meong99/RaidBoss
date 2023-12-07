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

	ECharacterAbilityInputs						GetAbilityInputID() const;
	FGameplayTagContainer						GetAbilityInputTags() const { return AbilityInputTags; }
	FGameplayTag								GetAbilityTriggerTag() const;
	
protected:
	void	SendGameplayEventToActor(AActor* Target, FGameplayTag EventTag, FGameplayEventData Payload);
	URaidBossAbilitySystemComponent*			GetOwnerAbilityComponent() const;
	const URaidBossCharacterStatusAttributeSet*	GetOwnerCharacterState() const;
	
protected:
	
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	//
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARaidBossCharacterBase>			OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	FGameplayTagContainer						AbilityInputTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	ECharacterAbilityInputs						AbilityInputID = ECharacterAbilityInputs::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	TArray<TObjectPtr<UAnimMontage>>			Montages; // 변경 필요 - 섹션으로 관리하도록
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Ability base")
	TArray<FGameplayEffectExecutionDefinition>	EffectExecutions; // 변경 필요 - 없애고 SetByCaller로 변경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Ability base")
	TArray<FGameplayModifierInfo>				EffectModifiers; // 변경 필요 - 없애고 SetByCaller로 변경

	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	//
};
