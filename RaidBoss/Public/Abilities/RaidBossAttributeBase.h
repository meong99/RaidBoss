#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "RaidBossAttributeBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)		\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) 				\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) 				\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ARaidBossCharacterBase;

UCLASS()
class RAIDBOSS_API URaidBossAttributeBase : public UAttributeSet
{
	GENERATED_BODY()
	
/*
*	----------- Overrided
*/
/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
protected:
	bool	SourceHasTag(const FGameplayEffectModCallbackData& Data, FName TagName) const;
	ARaidBossCharacterBase*	GetInstigator(const FGameplayEffectModCallbackData& Data) const;
	ARaidBossCharacterBase*	GetOwnerCharacter() const;
	float	GetModifiredSize(const FGameplayEffectModCallbackData& Data, const FGameplayAttribute& Attribute) const;
/*
*	----------- Access
*/
/*
*	----------- Member Variables
*/
};
