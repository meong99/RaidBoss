#include "Abilities/Item/RaidBossItemBase.h"

UTexture2D* URaidBossItemBase::GetItemTexture() const
{
	return ITemInfo.ItemTexture;
}

EITemCategory URaidBossItemBase::GetItemCategory() const
{
	return ITemInfo.ItemCategory;
}

FItemAbilityInformation URaidBossItemBase::GetItemInfo() const
{
	return ITemInfo;
}
