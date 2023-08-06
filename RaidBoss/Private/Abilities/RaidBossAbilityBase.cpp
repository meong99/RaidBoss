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

bool URaidBossAbilityBase::UseAbility() const
{
	URaidBossAbilitySystemComponent* AbilityComponent	= GetOwnerAbilityComponent();
	bool bAbilityActivated	= false;
	
	if (IsValid(AbilityComponent) == true)
	{
		AbilityComponent->TryActivateAbility(CurrentSpecHandle);
		bAbilityActivated = true;
	}

	return bAbilityActivated;
}

FGameplayEffectSpecHandle URaidBossAbilityBase::CreateEffectSpecHandle()
{
	FGameplayEffectContextHandle	EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(OwnerCharacter);
	
	UGameplayEffect* Effect = CreateNewEffect();
	if (Effect == nullptr)
		return FGameplayEffectSpecHandle();
	
	FGameplayEffectSpec* EffectSpec = new FGameplayEffectSpec(Effect, EffectContextHandle);

	return FGameplayEffectSpecHandle(EffectSpec);
}

UGameplayEffect* URaidBossAbilityBase::CreateNewEffect()
{
	if (EffectClass == nullptr)
		return nullptr;
	
	UGameplayEffect* EffectObject = NewObject<UGameplayEffect>(this, EffectClass);
	
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

ERaidBossAbilityInputID URaidBossAbilityBase::GetAbilityInputID() const
{
	return AbilityInputID;
}
