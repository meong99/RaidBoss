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

	virtual void	OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void	OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	bool						CanActivateAbility(FGameplayTag TriggerTag) const;
	bool						CanActivateAbility(int32 InputID);
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	bool						TryActivateAbilityByInputID(int32 InputID);
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	URaidBossAbilityBase*		GetAbilityByClass(TSubclassOf<URaidBossAbilityBase> AbilityClass);
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability System")
	URaidBossAbilityBase*		GetAbilityByInputID(int32 InputID);

	// 변경 필요 - 중복 확인은 좋으나 Spec을 받아서 GiveAbility를 하는게 확장성에 좋을 것 같음
	bool	GiveAbilityWithoutDuplication(TSubclassOf<URaidBossAbilityBase> AbilityClass,
				OUT FGameplayAbilitySpecHandle& OutSpecHandle, int32 InputID = INDEX_NONE);

/*
*	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
	const TMap<FGameplayTag, URaidBossAbilityBase*>&	GetInstanceAbilitiesByTag() const { return InstanceAbilitiesByTag; }

protected:
	
/*
 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	
	//
	
/*
 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	
	// GiveAbility 이후 인스턴싱된 Primary Ability를 TriggerTag별로 저장(태그별 하나만 저장)
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Ability System")
	TMap<FGameplayTag, URaidBossAbilityBase*>	InstanceAbilitiesByTag;
	
};
