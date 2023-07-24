#include "Abilities/Item/RaidBossConsumableItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

void URaidBossConsumableItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec*		AbilitySpec;

	AbilitySpec = GetCurrentAbilitySpec();
	FGameplayEffectSpecHandle EffectSpecHandle = CreateEffectSpecHandle();
	
	if (AbilitySpec && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool URaidBossConsumableItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return (bCanActivate);
}

FGameplayEffectSpecHandle URaidBossConsumableItem::CreateEffectSpecHandle()
{
	FGameplayEffectContextHandle	EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	FGameplayEffectSpec*			EffectSpec;
	UGameplayEffect*				Effect;
	
	Effect = CraeteNewEffectObject();
	EffectContextHandle.AddSourceObject(OwnerCharacter);
	
	EffectSpec = new FGameplayEffectSpec(Effect, EffectContextHandle);

	return FGameplayEffectSpecHandle(EffectSpec);
}

UGameplayEffect* URaidBossConsumableItem::CraeteNewEffectObject()
{
	UGameplayEffect* Effect;

	Effect = NewObject<UGameplayEffect>(this, ItemEffect);
	
	if (Effect)
	{
		Effect->Executions = ItemExecutions;
	}

	return Effect;
}
