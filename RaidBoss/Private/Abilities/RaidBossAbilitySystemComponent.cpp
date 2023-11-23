#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"

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

bool URaidBossAbilitySystemComponent::TryActivateAbilityByInputID(int32 InputID)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromInputID(InputID);
	
	if (Spec)
	{
		return TryActivateAbility(Spec->Handle);
	}

	return false;
}

FGameplayAbilitySpecHandle URaidBossAbilitySystemComponent::GiveAbilityToASC(TSubclassOf<URaidBossAbilityBase> AbilityClass,UObject* SourceObject, FGameplayTagContainer TagContainer)
{
	FGameplayAbilitySpec abilitySpec(AbilityClass.GetDefaultObject(), 1);
	abilitySpec.SourceObject = SourceObject;
	abilitySpec.DynamicAbilityTags.AppendTags(TagContainer);

	return GiveAbility(abilitySpec);
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
