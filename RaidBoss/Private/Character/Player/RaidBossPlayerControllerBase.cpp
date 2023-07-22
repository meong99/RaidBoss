#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossSkillSystem.h"
#include "Management/RaidBossInventorySystem.h"
#include "Management/RaidBossSkillSystem.h"

ARaidBossPlayerControllerBase::ARaidBossPlayerControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT(TEXT(
		"/Script/EnhancedInput.InputMappingContext'/Game/Input/IM_PlayerControl.IM_PlayerControl'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DefaultAttack(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_DefaultAttack.IA_DefaultAttack'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Look(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_Look.IA_Look'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_Jump.IA_Jump'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ToggleSkillWidget(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputWidget/IA_ToggleSkillWidget.IA_ToggleSkillWidget'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ToggleEquipWidget(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputWidget/IA_ToggleEquipWidget.IA_ToggleEquipWidget'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Interaction(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputGameControl/IA_Interaction.IA_Interaction'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Q(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_Q.IA_Q'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_E(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_E.IA_E'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_R(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_R.IA_R'"));
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Dash(TEXT(
		"/Script/EnhancedInput.InputAction'/Game/Input/InputSkill/IA_Dash.IA_Dash'"));
	
	if (DEFAULT_CONTEXT.Succeeded())
		InputActions.DefaultContext = DEFAULT_CONTEXT.Object;
	if (IA_DefaultAttack.Succeeded())
		InputActions.DefaultAttackAction = IA_DefaultAttack.Object;
	if (IA_Move.Succeeded())
		InputActions.MoveAction = IA_Move.Object;
	if (IA_Look.Succeeded())
		InputActions.LookAction = IA_Look.Object;
	if (IA_Jump.Succeeded())
		InputActions.JumpAction = IA_Jump.Object;
	if (IA_ToggleSkillWidget.Succeeded())
		InputActions.SkillWidgetAction = IA_ToggleSkillWidget.Object;
	if (IA_ToggleEquipWidget.Succeeded())
		InputActions.EquipWidgetAction = IA_ToggleEquipWidget.Object;
	if (IA_Interaction.Succeeded())
		InputActions.InteractionAction = IA_Interaction.Object;
	if (IA_Q.Succeeded())
		InputActions.QAction = IA_Q.Object;
	if (IA_E.Succeeded())
		InputActions.EAction = IA_E.Object;
	if (IA_R.Succeeded())
		InputActions.RAction = IA_R.Object;
	if (IA_Dash.Succeeded())
		InputActions.DashAction = IA_Dash.Object;
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
