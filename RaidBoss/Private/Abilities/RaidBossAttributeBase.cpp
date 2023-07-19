#include "Abilities/RaidBossAttributeBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Character/RaidBossCharacterBase.h"
#include "GameplayTagContainer.h"

bool URaidBossAttributeBase::SourceHasTag(const FGameplayEffectModCallbackData& Data, FName TagName) const
{
	FGameplayTagContainer	SourceTagContainer;
	FGameplayTag			Tag;

	Tag = FGameplayTag::RequestGameplayTag(TagName);
	SourceTagContainer = Data.EffectSpec.CapturedSourceTags.GetSpecTags();

	return SourceTagContainer.HasTag(Tag);
}

ARaidBossCharacterBase* URaidBossAttributeBase::GetInstigator(const FGameplayEffectModCallbackData& Data) const
{
	return Cast<ARaidBossCharacterBase>(Data.EffectSpec.GetEffectContext().GetInstigator());
}

ARaidBossCharacterBase* URaidBossAttributeBase::GetOwnerCharacter() const
{
	return  Cast<ARaidBossCharacterBase>(GetOwningActor());
}

float URaidBossAttributeBase::GetModifiredSize(const FGameplayEffectModCallbackData& Data, const FGameplayAttribute& Attribute) const
{
	for (auto Element : Data.EffectSpec.ModifiedAttributes)
	{
		if (Element.Attribute == Attribute)
		{
			return Element.TotalMagnitude;
		}
	}
	return 0;
}
