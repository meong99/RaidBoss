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
	URaidBossAbilitySystemComponent();
/*
*	----------- Overrided && Overload
*/
/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
	bool	GiveAbilityWithoutDuplication(TSubclassOf<URaidBossAbilityBase> AbilityClass,
		OUT FGameplayAbilitySpecHandle& OutSpecHandle, int32 InputID = INDEX_NONE);
/*
*	----------- Access
*/
	URaidBossAbilityBase*	GetAbilityByClass(TSubclassOf<URaidBossAbilityBase> AbilityClass);
	URaidBossAbilityBase*	GetAbilityByInputID(int32 InputID);
/*
*	----------- Member Variables
*/
};
