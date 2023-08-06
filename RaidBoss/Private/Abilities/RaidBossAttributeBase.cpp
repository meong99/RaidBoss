#include "Abilities/RaidBossAttributeBase.h"
#include "Character/RaidBossCharacterBase.h"

ARaidBossCharacterBase* URaidBossAttributeBase::GetOwnerCharacter() const
{
	return  Cast<ARaidBossCharacterBase>(GetOwningActor());
}
