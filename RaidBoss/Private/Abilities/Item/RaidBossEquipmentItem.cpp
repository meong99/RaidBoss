#include "Abilities/Item/RaidBossEquipmentItem.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"

TMap<EEquipType, URaidBossEquipmentItem*> URaidBossEquipmentItem::EquippedItems;

URaidBossEquipmentItem::URaidBossEquipmentItem()
{
}

void URaidBossEquipmentItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
	{
		if (IsEquipped() == true)
		{
			UnEquipItem();
		}
		else
		{
			EquipItem();
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool URaidBossEquipmentItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return (bCanActivate);
}

bool URaidBossEquipmentItem::IsEquipped() const
{
	bool	bIsEquipped = true;
	
	if (EquippedItems.Find(EquipType) == nullptr)
	{
		bIsEquipped = false;
	}
	
	return bIsEquipped;
}

int32 URaidBossEquipmentItem::GetEquipType() const
{
	return static_cast<int32>(EquipType);
}

const TMap<EEquipType, URaidBossEquipmentItem*>& URaidBossEquipmentItem::GetEquippedItems()
{
	return EquippedItems;
}

void URaidBossEquipmentItem::EquipItem()
{
	for (auto Element : Effects)
	{
		UGameplayEffect*	Effect = Element.GetDefaultObject();
		FActiveGameplayEffectHandle Handle = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Effect, 1);
		EffectHandleArray.Add(Handle);
	}
	
	EquippedItems.FindOrAdd(EquipType, this);
}

void URaidBossEquipmentItem::UnEquipItem()
{
	URaidBossEquipmentItem* EquippedItem = *EquippedItems.Find(EquipType);
	if (IsValid(EquippedItem) == true)
	{
		EquippedItem->RemoveAppliedItemEffect();
	}
	
	EquippedItems.Remove(EquipType);
}

void URaidBossEquipmentItem::RemoveAppliedItemEffect()
{
	for (auto Element : EffectHandleArray)
	{
		URaidBossAbilitySystemComponent* OwnerAbilityComponent = GetOwnerAbilityComponent();

		if (IsValid(OwnerAbilityComponent) == true)
			OwnerAbilityComponent->RemoveActiveGameplayEffect(Element);
	}
}
