#include "Character/RaidBossCharacterBase.h"

#include "Abilities/RaidBossAbilityBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

ARaidBossCharacterBase::ARaidBossCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -98.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());

	AbilitySystemComponent = CreateDefaultSubobject<URaidBossAbilitySystemComponent>(TEXT("Ability Component"));
	CharacterStatusAttributeSet = CreateDefaultSubobject<URaidBossCharacterStatusAttributeSet>(TEXT("Character Status AttributeSet"));
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::Alive);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanMove);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanUsingAttack);
}

void ARaidBossCharacterBase::ClearAllMember()
{
	AbilitySystemComponent = nullptr;
}

void ARaidBossCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	GiveDefaultAbilities();
}

void ARaidBossCharacterBase::GiveDefaultAbilities()
{
	if (AbilitySystemComponent == nullptr)
		return;
	
	for (const auto Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle	AbilitySpecHandle;
		URaidBossAbilityBase*		AbilityCDO = Cast<URaidBossAbilityBase>(Ability->GetDefaultObject());
		
		AbilitySystemComponent->GiveAbilityWithoutDuplication(Ability, AbilitySpecHandle, static_cast<int32>(AbilityCDO->AbilityInputID));
	}
}

void ARaidBossCharacterBase::ApplyCharacterStatusEffect()
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

		ContextHandle.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(CharacterStatusEffect.GetDefaultObject(), 1, ContextHandle);
	}
}

UAbilitySystemComponent* ARaidBossCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URaidBossAbilitySystemComponent* ARaidBossCharacterBase::GetRaidBossAbilitySystemComponent() const
{
	URaidBossAbilitySystemComponent* Ret = Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponent());

	if (IsValid(Ret) == false)
		return nullptr;

	return Ret;
}

const URaidBossCharacterStatusAttributeSet* ARaidBossCharacterBase::GetCharacterStatusAttributeSet() const
{
	return CharacterStatusAttributeSet;
}

double ARaidBossCharacterBase::GetExperience() const
{
	return Experience;
}

double ARaidBossCharacterBase::GetMaxExperience() const
{
	return MaxExperience;
}

bool ARaidBossCharacterBase::IsCharacterStateTurnOn(ECharacterState CharacterState)
{
	return CharacterStateBitMask & static_cast<int32>(CharacterState);
}

float ARaidBossCharacterBase::GetHealth() const
{
	return CharacterStatusAttributeSet->GetHealth();
}

float ARaidBossCharacterBase::GetAttackPower() const
{
	return CharacterStatusAttributeSet->GetAttackPower();
}

void ARaidBossCharacterBase::TurnOnCharacterStateBitMap(ECharacterState CharacterState)
{
	int32	Bit = static_cast<int32>(CharacterState);
	CharacterStateBitMask |= Bit;
}

void ARaidBossCharacterBase::TurnOffCharacterStateBitMap(ECharacterState CharacterState)
{
	int32	Bit = static_cast<int32>(CharacterState);
	CharacterStateBitMask &= ~Bit;
}

void ARaidBossCharacterBase::GiveExperience(double Exp)
{
	Experience += Exp;

	if (Experience >= MaxExperience)
	{
		CharacterLevelUp(floor(Experience / MaxExperience));
	}
}

void ARaidBossCharacterBase::CharacterLevelUp(float IncrementNum)
{
	Experience = fmod(Experience, MaxExperience);
	MaxExperience = MaxExperience * IncrementNum * 2;
	CharacterLevel += IncrementNum;
}
