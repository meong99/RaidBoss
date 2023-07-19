#include "Abilities/RaidBossAbilityBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

URaidBossAbilityBase::URaidBossAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URaidBossAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	OwnerCharacter = Cast<ARaidBossCharacterBase>(ActorInfo->OwnerActor);
}

URaidBossAbilitySystemComponent* URaidBossAbilityBase::GetOwnerAbilityComponent() const
{
	return Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

const URaidBossCharacterStatusAttributeSet* URaidBossAbilityBase::GetOwnerCharacterState()
{
	const URaidBossCharacterStatusAttributeSet* OwnerCharacterState = nullptr;
	
	if (IsValid(OwnerCharacter) == true)
	{
		OwnerCharacterState = OwnerCharacter->GetCharacterStatusAttributeSet();
	}

	return OwnerCharacterState;
}

FGameplayEffectSpecHandle URaidBossAbilityBase::SetCallerMagnitudeByDataTag(TSubclassOf<UGameplayEffect> Effect, const FGameplayTag& DataTag, float Magnitude, float Level)
{
	FGameplayEffectSpecHandle Handle = MakeOutgoingGameplayEffectSpec(Effect, Level);
	FGameplayEffectSpec* Spec = Handle.Data.Get();

	if (Spec)
	{
		Spec->SetSetByCallerMagnitude(DataTag, Magnitude);
	}

	return Handle;
}

bool URaidBossAbilityBase::UseAbility()
{
	URaidBossAbilitySystemComponent*	AbilityComponent;
	bool								bAbilityActivated;
	
	AbilityComponent	= GetOwnerAbilityComponent();
	bAbilityActivated	= false;
	
	if (IsValid(AbilityComponent) == true)
	{
		AbilityComponent->TryActivateAbility(CurrentSpecHandle);
		bAbilityActivated = true;
	}

	return bAbilityActivated;
}
