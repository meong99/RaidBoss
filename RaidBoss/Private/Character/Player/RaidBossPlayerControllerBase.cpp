#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossSkillSystem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Management/RaidBossRewardSystem.h"

ARaidBossPlayerControllerBase::ARaidBossPlayerControllerBase()
{
	RewardSystem = CreateDefaultSubobject<URaidBossRewardSystem>(TEXT("Reward System"));
}

void ARaidBossPlayerControllerBase::MoveCharacter(FVector2D Value) const
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

	if (PlayerBase)
		PlayerBase->MoveCharacter(Value);
}

void ARaidBossPlayerControllerBase::JumpCharacter() const
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

	if (PlayerBase)
		PlayerBase->JumpCharacter();
}

void ARaidBossPlayerControllerBase::StopJumpCharacter() const
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

	if (PlayerBase)
		PlayerBase->StopJumpCharacter();
}

void ARaidBossPlayerControllerBase::LookCharacter(FVector2D Value) const
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

	if (PlayerBase)
		PlayerBase->LookCharacter(Value);
}

ARaidBossPlayerBase* ARaidBossPlayerControllerBase::GetRaidBossPlayerBase() const
{
	return Cast<ARaidBossPlayerBase>(GetPawn());
}

URaidBossAbilitySystemComponent* ARaidBossPlayerControllerBase::GetRaidBossAbilitySystemComponent() const
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();
	
	return PlayerBase ? PlayerBase->GetRaidBossAbilitySystemComponent() : nullptr;
}

URaidBossInventorySystem* ARaidBossPlayerControllerBase::GetInventorySystem() const
{
	return InventorySystem;
}

void ARaidBossPlayerControllerBase::ToggleInventoryWidget() const
{
	InventorySystem->ToggleInventoryWidget();
}

void ARaidBossPlayerControllerBase::ToggleSkillWidget() const
{
	SkillSystem->ToggleSkillWidget();
}

void ARaidBossPlayerControllerBase::AttemptDropItem(EITemCategory ItemCategory, int32 Index)
{
	if (InventorySystem)
	{
		InventorySystem->RemoveItem(ItemCategory, Index);
	}
}
