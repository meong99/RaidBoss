#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Management/RaidBossGameplayTags.h"

URaidBossEquipmentItem::URaidBossEquipmentItem()
{
}

bool URaidBossEquipmentItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return (bCanActivate);
}

void URaidBossEquipmentItem::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	UnEquipItem();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

int32 URaidBossEquipmentItem::GetEquipType() const
{
	return static_cast<int32>(EquipType);
}

void URaidBossEquipmentItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData->InstigatorTags.HasTag(RaidBossGameplayTags::Get().Event_Equipment_ToRemove))
	{
		UnEquipItem();
		bIsThisArmed = false;
	}
	else if (bIsThisArmed == false)
	{
		EquipItem();
		bIsThisArmed = true;
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URaidBossEquipmentItem::EquipItem()
{
	FGameplayAbilitySpec* CurrentAbilitySpec = GetCurrentAbilitySpec();

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ItemEffect);
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackPower, AdditiveAttackPower);
	
	if (CurrentAbilitySpec && OwnerCharacter)
	{
		CurrentAbilitySpec->GameplayEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);

		OwnerCharacter->NotifyEquipmentChanged.Broadcast(GetAbilityTriggerTag(), static_cast<int32>(EquipType), GetItemTexture());
		OwnerCharacter->DecreaseOrRemoveInventoryData(GetAbilityTriggerTag());
	}
}

void URaidBossEquipmentItem::UnEquipItem()
{
	FGameplayAbilitySpec*		CurrentAbilitySpec = GetCurrentAbilitySpec();
	UAbilitySystemComponent*	AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	if (CurrentAbilitySpec && AbilitySystemComponent && OwnerCharacter)
	{
		FActiveGameplayEffectHandle EffectHandle = CurrentAbilitySpec->GameplayEffectHandle;
		
		AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
		
		OwnerCharacter->NotifyEquipmentChanged.Broadcast(FGameplayTag{}, static_cast<int32>(EquipType), nullptr);
		OwnerCharacter->IncreaseOrAddInventoryData(GetItemKey());
	}
}
