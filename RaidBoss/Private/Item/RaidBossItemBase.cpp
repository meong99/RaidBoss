#include "Item/RaidBossItemBase.h"
#include "AbilitySystemGlobals.h"

URaidBossItemBase::URaidBossItemBase()
{
}

void URaidBossItemBase::ConstructedByOwner(AActor* Owner)
{
	FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
	EffectContext->AddInstigator(Owner, Owner);

	for (auto& Effects : ItemEffects)
	{
		SpecHandles.Add(FGameplayEffectSpecHandle(new FGameplayEffectSpec(Effects.GetDefaultObject(), FGameplayEffectContextHandle(EffectContext), 1.f)));
	}

	OwnerActor = Owner;
}
