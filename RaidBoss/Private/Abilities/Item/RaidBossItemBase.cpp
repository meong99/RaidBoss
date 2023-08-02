#include "Abilities/Item/RaidBossItemBase.h"

const TObjectPtr<UTexture2D> URaidBossItemBase::GetItemTexture() const
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
