#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Global/RaidBoss.h"
#include "Global/RaidBossGameInstance.h"

URaidBossAbilitySystemComponent::URaidBossAbilitySystemComponent()
{
}

bool URaidBossAbilitySystemComponent::GiveAbilityWithoutDuplication(TSubclassOf<URaidBossAbilityBase> AbilityClass,
	FGameplayAbilitySpecHandle& OutSpecHandle, int32 InputID)
{
	if (FindAbilitySpecFromClass(AbilityClass) == nullptr)
	{
		FGameplayAbilitySpec Spec {AbilityClass, 1, InputID};

		OutSpecHandle = GiveAbility(Spec);
	}
	return false;
}

URaidBossAbilityBase* URaidBossAbilitySystemComponent::GetAbilityByClass(TSubclassOf<URaidBossAbilityBase> AbilityClass)
{
	URaidBossAbilityBase*	Ret = nullptr;
	
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass);
	if (Spec)
	{
		Ret = Cast<URaidBossAbilityBase>(Spec->GetPrimaryInstance());
	}

	return Ret;
}

URaidBossAbilityBase* URaidBossAbilitySystemComponent::GetAbilityByInputID(int32 InputID)
{
	URaidBossAbilityBase*	Ret = nullptr;
	
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Ret = Cast<URaidBossAbilityBase>(Spec->GetPrimaryInstance());
	}

	return Ret;
}
