#include "Shared/RaidBossSharedCharacter.h"

ARaidBossSharedCharacter::ARaidBossSharedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -98.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());

	AbilitySystemComponent = CreateDefaultSubobject<URaidBossAbilitySystemComponent>("Ability Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<URaidBossDefaultAttributeSet>("Default Attribute Set");
}

void ARaidBossSharedCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*SpecHandle.Data.Get());
		}
	}
}

void ARaidBossSharedCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const TSubclassOf<URaidBossSharedAbility>& StartupAbility : DefaultAbilities)
		{
			FGameplayAbilitySpecHandle& SpecHandle = HandleOfDefaultAbilities.FindOrAdd(StartupAbility.GetDefaultObject()->AbilityInputID);

			SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
		for (const TSubclassOf<URaidBossSharedAbility>& StartupAbility : Skils)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ARaidBossSharedCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}

UAbilitySystemComponent* ARaidBossSharedCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URaidBossAbilitySystemComponent* ARaidBossSharedCharacter::GetRaidBossAbilitySystemComponent() const
{
	URaidBossAbilitySystemComponent* Ret = Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!Ret)
		return nullptr;
	return Ret;
}
