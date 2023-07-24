#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossSkillSystem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Management/RaidBossSkillSystem.h"

ARaidBossPlayerControllerBase::ARaidBossPlayerControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DefaultAttack(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_DefaultAttack.IA_DefaultAttack'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Q(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_Q.IA_Q'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_E(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_E.IA_E'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_R(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_R.IA_R'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Dash(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_Dash.IA_Dash'"));
	
	if (IA_DefaultAttack.Succeeded())
		InputActions.DefaultAttackAction = IA_DefaultAttack.Object;
	if (IA_Q.Succeeded())
		InputActions.QAction = IA_Q.Object;
	if (IA_E.Succeeded())
		InputActions.EAction = IA_E.Object;
	if (IA_R.Succeeded())
		InputActions.RAction = IA_R.Object;
	if (IA_Dash.Succeeded())
		InputActions.DashAction = IA_Dash.Object;
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

const FRaidBossInputAction& ARaidBossPlayerControllerBase::GetInputAction() const
{
	return InputActions;
}

void ARaidBossPlayerControllerBase::ToggleInventoryWidget() const
{
	InventorySystem->ToggleInventoryWidget();
}

void ARaidBossPlayerControllerBase::ToggleSkillWidget() const
{
	SkillSystem->ToggleSkillWidget();
}
