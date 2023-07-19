#include "Character/RaidBossCharacterBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities\RaidBossAbilityBase.h"
#include "Global/RaidBossGameInstance.h"

ARaidBossCharacterBase::ARaidBossCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -98.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());

	AbilitySystemComponent = CreateDefaultSubobject<URaidBossAbilitySystemComponent>("Ability Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	CharacterStatusAttributeSet = CreateDefaultSubobject<URaidBossCharacterStatusAttributeSet>("Character Status AttributeSet");
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::Alive);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanMove);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanUsingAttack);
}


void ARaidBossCharacterBase::ClearAllMember()
{
	AbilitySystemComponent = nullptr;
}

void ARaidBossCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
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

int ARaidBossCharacterBase::GetSkillPoint() const
{
	return SkillPoint;
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

void ARaidBossCharacterBase::IncreaseSkillPoint()
{
	SkillPoint++;
}

void ARaidBossCharacterBase::DecreaseSkillPoint()
{
	SkillPoint--;
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
	SkillPoint += 5;
}
