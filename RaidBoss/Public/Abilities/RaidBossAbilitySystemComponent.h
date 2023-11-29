#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RaidBossAbilitySystemComponent.generated.h"

class URaidBossAbilityBase;
class URaidBossSkillBase;
class URaidBossGameInstance;

UCLASS()
class RAIDBOSS_API URaidBossAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	bool	CanActivateAbility(FGameplayTag TriggerTag) const;
	
	bool	GiveAbilityWithoutDuplication(TSubclassOf<URaidBossAbilityBase> AbilityClass,
				OUT FGameplayAbilitySpecHandle& OutSpecHandle, int32 InputID = INDEX_NONE);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	bool	TryActivateAbilityByInputID(int32 InputID);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	URaidBossAbilityBase*	GetAbilityByClass(TSubclassOf<URaidBossAbilityBase> AbilityClass);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	URaidBossAbilityBase*	GetAbilityByInputID(int32 InputID);


	const TMap<FGameplayTag, URaidBossAbilityBase*>&	GetInstanceAbilitiesByTag() const { return InstanceAbilitiesByTag; }
	//임시
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	FGameplayAbilitySpecHandle	GiveAbilityToASC(TSubclassOf<URaidBossAbilityBase> AbilityClass, UObject* SourceObject, FGameplayTagContainer TagContainer);

protected:
	
	/*
	 *	Changed on Initialize * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Ability System")
	TMap<FGameplayTag, URaidBossAbilityBase*>	InstanceAbilitiesByTag;
	
	/*
	 *	Changed on every cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
};
