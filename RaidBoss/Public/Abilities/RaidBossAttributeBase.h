﻿#pragma once

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

protected:
	ARaidBossCharacterBase*	GetOwnerCharacter() const;
};
