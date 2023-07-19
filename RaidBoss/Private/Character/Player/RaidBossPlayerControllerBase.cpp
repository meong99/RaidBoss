#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "Widget/RaidBossUserWidget.h"
#include "InputMappingContext.h"
#include "Management/RaidBossSkillSystem.h"
#include "UI/RaidBossInventorySystem.h"
#include "Management/RaidBossSkillSystem.h"

ARaidBossPlayerControllerBase::ARaidBossPlayerControllerBase()
{
	InventorySystem	= CreateDefaultSubobject<URaidBossInventorySystem>(FName("Inventory System"));
	SkillSystem		= CreateDefaultSubobject<URaidBossSkillSystem>(FName("Skill System"));
	
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

void ARaidBossPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	CreatePlayerWidget();
	AddDefaultWidgetToViewport();
}

void ARaidBossPlayerControllerBase::CreatePlayerWidget()
{
	if (IsValid(PlayerStateWidgetClass) == true)
		PlayerStateWidget = CreateWidget<URaidBossUserWidget>(this, PlayerStateWidgetClass, "PlayerStateWidgetClass");
	if (IsValid(CrossHairWidgetClass) == true)
		CrossHairWidget = CreateWidget<URaidBossUserWidget>(this, CrossHairWidgetClass, "CrossHairWidget");
	if (IsValid(QuickSlotWidgetClass) == true)
		QuickSlotWidget = CreateWidget<URaidBossUserWidget>(this, QuickSlotWidgetClass, "QuickSlotWidget");
	if (IsValid(SkillWidgetClass) == true)
		SkillWidget = CreateWidget<URaidBossUserWidget>(this, SkillWidgetClass, "SkillWidget");
	if (IsValid(EquipWidgetClass) == true)
		EquipWidget = CreateWidget<URaidBossUserWidget>(this, EquipWidgetClass, "EquipWidget");
}

void ARaidBossPlayerControllerBase::AddDefaultWidgetToViewport() const
{
	if (IsValid(PlayerStateWidget))
		PlayerStateWidget->AddToViewport();

	if (IsValid(CrossHairWidget))
		CrossHairWidget->AddToViewport();
}

void ARaidBossPlayerControllerBase::RemoveDefaultWidgetFromViewport() const
{
	if (IsValid(PlayerStateWidget))
		PlayerStateWidget->RemoveFromParent();

	if (IsValid(QuickSlotWidget))
		QuickSlotWidget->RemoveFromParent();

	if (IsValid(CrossHairWidget))
		CrossHairWidget->RemoveFromParent();
}

TArray<TSubclassOf<URaidBossSkillBase>> ARaidBossPlayerControllerBase::GetSkillClasses() const
{
	return SkillClasses;
}

TSubclassOf<UGameplayEffect> ARaidBossPlayerControllerBase::GetCharacterStatusEffect() const
{
	return CharacterStatusEffect;
}

const FRaidBossInputAction& ARaidBossPlayerControllerBase::GetInputAction() const
{
	return InputActions;
}

URaidBossUserWidget* ARaidBossPlayerControllerBase::GetPlayerStateWidget() const
{
	return PlayerStateWidget;
}

URaidBossUserWidget* ARaidBossPlayerControllerBase::GetCrossHairWidget() const
{
	return CrossHairWidget;
}

URaidBossUserWidget* ARaidBossPlayerControllerBase::GetQuickSlotWidget() const
{
	return QuickSlotWidget;
}

URaidBossUserWidget* ARaidBossPlayerControllerBase::GetSkillWidget() const
{
	return SkillWidget;
}

URaidBossUserWidget* ARaidBossPlayerControllerBase::GetEquipWidget() const
{
	return EquipWidget;
}

void ARaidBossPlayerControllerBase::ToggleInventoryWidget() const
{
	InventorySystem->ToggleInventoryWidget();
}

void ARaidBossPlayerControllerBase::ToggleSkillWidget() const
{
	SkillSystem->ToggleSkillWidget();
}
