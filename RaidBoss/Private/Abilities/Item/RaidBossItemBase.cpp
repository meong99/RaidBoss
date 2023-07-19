#include "Abilities/Item/RaidBossItemBase.h"

URaidBossItemBase::URaidBossItemBase()
{
}

const TObjectPtr<UTexture2D> URaidBossItemBase::GetItemTexture() const
{
	return ITemInfo.ItemTexture;
}

EITemCategory URaidBossItemBase::GetItemCategory() const
{
	return ITemInfo.ItemCategory;
}

//
// FGameplayEffectSpecHandle URaidBossItemBase::AddAllMagnitudeOfEffect(URaidBossAbilitySystemComponent* AbilityComponent)
// {
// 	FGameplayEffectContextHandle Context = AbilityComponent->MakeEffectContext();
// 	FGameplayEffectSpecHandle Handle = AbilityComponent->MakeOutgoingSpec(ITemInfo.ItemEffect, 1, Context);
// 	FGameplayEffectSpec*		Spec = Handle.Data.Get();
// 	FGameplayTag DataTag;
// 	
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.Health"));
// 	Spec->SetSetByCallerMagnitude(DataTag, Health);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.MaxHealth"));
// 	Spec->SetSetByCallerMagnitude(DataTag, MaxHealth);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.Mana"));
// 	Spec->SetSetByCallerMagnitude(DataTag, Mana);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.MaxMana"));
// 	Spec->SetSetByCallerMagnitude(DataTag, MaxMana);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.AttackPower"));
// 	Spec->SetSetByCallerMagnitude(DataTag, AttackPower);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.DefensePower"));
// 	Spec->SetSetByCallerMagnitude(DataTag, DefensePower);
//
// 	return Handle;
// }
//
// FGameplayEffectSpecHandle URaidBossItemBase::SubAllMagnitudeOfEffect(URaidBossAbilitySystemComponent* AbilityComponent)
// {
// 	FGameplayEffectContextHandle Context = AbilityComponent->MakeEffectContext();
// 	FGameplayEffectSpecHandle Handle = AbilityComponent->MakeOutgoingSpec(ITemInfo.ItemEffect, 1, Context);
// 	FGameplayEffectSpec*		Spec = Handle.Data.Get();
// 	FGameplayTag DataTag;
//
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.Health"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -Health);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.MaxHealth"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -MaxHealth);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.Mana"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -Mana);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.MaxMana"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -MaxMana);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.AttackPower"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -AttackPower);
// 	DataTag = FGameplayTag::RequestGameplayTag(FName("Status.DefensePower"));
// 	Spec->SetSetByCallerMagnitude(DataTag, -DefensePower);
//
// 	return Handle;
// }
