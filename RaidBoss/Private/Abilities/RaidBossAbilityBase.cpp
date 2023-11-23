#include "Abilities/RaidBossAbilityBase.h"
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

	if (OwnerCharacter)
	{
		float AttackSpeed = OwnerCharacter->GetCharacterStatusAttributeSet()->AttackSpeed.GetCurrentValue();
	}
}

void URaidBossAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	
	if (CooldownGE && OwnerCharacter)
	{
		float AttackSpeed = OwnerCharacter->GetCharacterStatusAttributeSet()->AttackSpeed.GetCurrentValue();
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().CoolDown_ByAttackSpeed, AttackSpeed);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

FGameplayTagContainer URaidBossAbilityBase::GetAbilityTags()
{
	return AbilityTags;
}

FGameplayTagContainer URaidBossAbilityBase::GetBlockAbilityTags()
{
	return BlockAbilitiesWithTag;
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

bool URaidBossAbilityBase::CanActivateAbilityForBP() const
{
	return CanActivateAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
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

ECharacterAbilityInputs URaidBossAbilityBase::GetAbilityInputID() const
{
	return AbilityInputID;
}
