#include "Abilities/RaidBossAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Management/RaidBossGameplayTags.h"

URaidBossAbilityBase::URaidBossAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URaidBossAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	OwnerCharacter = Cast<ARaidBossCharacterBase>(ActorInfo->OwnerActor);
}

FGameplayTag URaidBossAbilityBase::GetAbilityTriggerTag() const
{
	if (AbilityTriggers.IsEmpty())
	{
		return FGameplayTag{};
	}
	
	FAbilityTriggerData TriggerData = *AbilityTriggers.begin();

	return TriggerData.TriggerTag;
}

void URaidBossAbilityBase::SendGameplayEventToActor(AActor* Target, FGameplayTag EventTag, FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, EventTag, Payload);
}

URaidBossAbilitySystemComponent* URaidBossAbilityBase::GetOwnerAbilityComponent() const
{
	return Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

const URaidBossCharacterStatusAttributeSet* URaidBossAbilityBase::GetOwnerCharacterState() const
{
	if (IsValid(OwnerCharacter) == true)
	{
		return OwnerCharacter->GetCharacterStatusAttributeSet();
	}

	return nullptr;
}
