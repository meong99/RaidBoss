#include "Management/RaidBossRewardSystem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"

URaidBossRewardSystem::URaidBossRewardSystem()
{
}

void URaidBossRewardSystem::NewReward(TArray<TSubclassOf<URaidBossItemBase>> Items, int32 Gold)
{
	ARaidBossPlayerControllerBase*	PlayerController = GetRaidBossPlayerControllerBase();
	URaidBossInventorySystem*		InventorySystem = PlayerController ? PlayerController->GetInventorySystem() : nullptr;
	
	if (InventorySystem == nullptr)
		return;

	for (const auto NewItem : Items)
	{
		InventorySystem->AddNewItem(NewItem);
	}
	InventorySystem->AddGold(Gold);
}

ARaidBossPlayerControllerBase* URaidBossRewardSystem::GetRaidBossPlayerControllerBase() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetOwner());
}
