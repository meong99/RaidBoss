#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"

void URaidBossAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	URaidBossAbilityBase*	AbilityBase = Cast<URaidBossAbilityBase>(AbilitySpec.GetPrimaryInstance());
	
	if (AbilityBase && AbilityBase->GetAbilityTriggerTag().IsValid())
	{
		InstanceAbilitiesByTag.Add(AbilityBase->GetAbilityTriggerTag(), AbilityBase);
	}
}

void URaidBossAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	URaidBossAbilityBase*	AbilityBase = Cast<URaidBossAbilityBase>(AbilitySpec.GetPrimaryInstance());
	
	if (AbilityBase && AbilityBase->GetAbilityTriggerTag().IsValid())
	{
		InstanceAbilitiesByTag.Remove(AbilityBase->GetAbilityTriggerTag());
	}
	
	Super::OnRemoveAbility(AbilitySpec);
}

bool URaidBossAbilitySystemComponent::CanActivateAbility(FGameplayTag TriggerTag) const
{
	URaidBossAbilityBase*	AbilityBase = InstanceAbilitiesByTag.FindRef(TriggerTag);

	if (AbilityBase)
	{
		return AbilityBase->CanActivateAbility(AbilityBase->GetCurrentAbilitySpecHandle(), AbilityBase->GetCurrentActorInfo());
	}

	return false;
}

bool URaidBossAbilitySystemComponent::GiveAbilityWithoutDuplication(TSubclassOf<URaidBossAbilityBase> AbilityClass,
                                                                    FGameplayAbilitySpecHandle& OutSpecHandle, int32 InputID)
{
	if (FindAbilitySpecFromClass(AbilityClass) == nullptr)
	{
		FGameplayAbilitySpec Spec {AbilityClass, 1, InputID};

		OutSpecHandle = GiveAbility(Spec);

		return true;
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
