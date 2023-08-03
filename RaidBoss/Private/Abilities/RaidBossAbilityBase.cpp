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

FGameplayEffectSpecHandle URaidBossAbilityBase::CreateEffectSpecHandle()
{
	FGameplayEffectContextHandle	EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	FGameplayEffectSpec*			EffectSpec;
	UGameplayEffect*				Effect;
	
	Effect = CraeteNewEffect();
	EffectContextHandle.AddSourceObject(OwnerCharacter);
	EffectSpec = new FGameplayEffectSpec(Effect, EffectContextHandle);

	return FGameplayEffectSpecHandle(EffectSpec);
}

UGameplayEffect* URaidBossAbilityBase::CraeteNewEffect()
{
	UGameplayEffect* EffectObject;

	EffectObject = NewObject<UGameplayEffect>(this, EffectClass);
	
	if (EffectObject)
	{
		if (EffectExecutions.IsEmpty() == false)
		{
			EffectObject->Executions.Append(EffectExecutions);
		}
		else if (EffectModifiers.IsEmpty() == false)
		{
			EffectObject->Modifiers.Append(EffectModifiers);
		}
	}

	return EffectObject;
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
