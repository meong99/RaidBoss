#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossSkillSystem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Management/RaidBossSkillSystem.h"

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

void ARaidBossPlayerControllerBase::Interaction()
{
	ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

	if (PlayerBase)
		PlayerBase->Interaction();
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
	URaidBossAbilitySystemComponent*	AbilitySystemComponent;
	ARaidBossPlayerBase*				PlayerBase;

	PlayerBase = GetRaidBossPlayerBase();
	AbilitySystemComponent = PlayerBase ? PlayerBase->GetRaidBossAbilitySystemComponent() : nullptr;

	return AbilitySystemComponent;
}

TSubclassOf<UGameplayEffect> ARaidBossPlayerControllerBase::GetCharacterStatusEffect() const
{
	return CharacterStatusEffect;
}

void ARaidBossPlayerControllerBase::ToggleInventoryWidget() const
{
	InventorySystem->ToggleInventoryWidget();
}

void ARaidBossPlayerControllerBase::ToggleSkillWidget() const
{
	SkillSystem->ToggleSkillWidget();
}
