#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossDefaultAttributeSet.h"
#include "Shared/RaidBossSharedAbility.h"
#include "RaidBoss/RaidBoss.h"
#include "RaidBossSharedCharacter.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossSharedCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARaidBossSharedCharacter();
	
protected:
	virtual void	PossessedBy(AController* NewController) override;
	virtual void	InitializeAttributes();
	virtual void	GiveAbilities();

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void	CallOwnerForTryAbility();

	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponent*					GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	URaidBossAbilitySystemComponent*			GetRaidBossAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable)
	const URaidBossDefaultAttributeSet*			GetAttributeSet() const { return AttributeSet; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayEffect>				GetDefaultAttributeEffect() const { return DefaultAttributeEffect; }
	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<URaidBossSharedAbility>>	GetDefaultAbilities() const { return DefaultAbilities; }
	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<URaidBossSharedAbility>>	GetSkills() const { return Skils; }
	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle					GetHandleOfDefaultAbilities(ERaidBossAbilityInputID InputID) const { return HandleOfDefaultAbilities[InputID]; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<URaidBossSharedAbility>>					DefaultAbilities;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<URaidBossAbilitySystemComponent>					AbilitySystemComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<const URaidBossDefaultAttributeSet>				AttributeSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect>								DefaultAttributeEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<URaidBossSharedAbility>>					Skils;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TMap<ERaidBossAbilityInputID,FGameplayAbilitySpecHandle>	HandleOfDefaultAbilities;
}; 
