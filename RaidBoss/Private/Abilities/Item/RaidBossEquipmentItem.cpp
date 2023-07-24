﻿#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

URaidBossEquipmentItem::URaidBossEquipmentItem()
{
	bRetriggerInstancedAbility = true;
}

void URaidBossEquipmentItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		EquipItem();
	}
}

void URaidBossEquipmentItem::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	UnEquipItem();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

bool URaidBossEquipmentItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return (bCanActivate);
}

void URaidBossEquipmentItem::EquipItem()
{
	FGameplayAbilitySpec*	AbilitySpec;
	
	AbilitySpec = GetCurrentAbilitySpec();
	FGameplayEffectSpecHandle EffectSpecHandle = CreateEffectSpecHandle();

	if (AbilitySpec)
	{
		AbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}
}

void URaidBossEquipmentItem::UnEquipItem()
{
	FGameplayAbilitySpec*		AbilitySpec = GetCurrentAbilitySpec();
	UAbilitySystemComponent*	AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	if (AbilitySpec && AbilitySystemComponent)
	{
		FActiveGameplayEffectHandle EffectHandle = AbilitySpec->GameplayEffectHandle;
		
		AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
	}
}

FGameplayEffectSpecHandle URaidBossEquipmentItem::CreateEffectSpecHandle()
{
	const URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	FGameplayEffectSpec*					EffectSpec;
	UGameplayEffect*						EffectObject;
	
	AbilitySystemComponent = Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle =
		AbilitySystemComponent ? AbilitySystemComponent->MakeEffectContext() : FGameplayEffectContextHandle();
	
	EffectObject = CraeteNewEffectObject();
	EffectContextHandle.AddSourceObject(OwnerCharacter);
	
	EffectSpec = new FGameplayEffectSpec(EffectObject, EffectContextHandle);

	return FGameplayEffectSpecHandle(EffectSpec);
}

UGameplayEffect* URaidBossEquipmentItem::CraeteNewEffectObject()
{
	UGameplayEffect* Effect;

	Effect = NewObject<UGameplayEffect>(this, ItemEffect);
	
	if (Effect)
	{
		Effect->Modifiers = EquipModifiers;
	}

	return Effect;
}

int32 URaidBossEquipmentItem::GetEquipType() const
{
	return static_cast<int32>(EquipType);
}
