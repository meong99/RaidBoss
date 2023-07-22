#include "Abilities/Item/RaidBossEquipmentItem.h"
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

int32 URaidBossEquipmentItem::GetEquipType() const
{
	return static_cast<int32>(EquipType);
}

void URaidBossEquipmentItem::EquipItem()
{
	FGameplayAbilitySpec*	Spec;
	UGameplayEffect*		EffectToApply;
	
	Spec = GetCurrentAbilitySpec();
	EffectToApply = ItemEffect.GetDefaultObject();
	
	
	if (Spec && EffectToApply)
	{
		EffectToApply->Modifiers = ItemModifiers;
		Spec->GameplayEffectHandle = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectToApply, 1);
	}
}

void URaidBossEquipmentItem::UnEquipItem()
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	FGameplayAbilitySpec*				Spec;
	
	Spec = GetCurrentAbilitySpec();
	AbilitySystemComponent = Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	
	if (Spec && AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Spec->GameplayEffectHandle);
	}
}
