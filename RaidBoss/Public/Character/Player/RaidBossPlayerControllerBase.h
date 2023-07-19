#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RaidBossPlayerControllerBase.generated.h"

class URaidBossInventorySystem;
class URaidBossSkillSystem;
class URaidBossAbilityBase;
class UGameplayEffect;
class URaidBossItemBase;
class URaidBossSkillBase;
class URaidBossUserWidget;
class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FRaidBossInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputMappingContext>	DefaultContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			DefaultAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			SkillWidgetAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			EquipWidgetAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			InteractionAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			QAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			EAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			RAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			DashAction;
};

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	ARaidBossPlayerControllerBase();
/*
 *	----------- Overrided
 */
protected:
	virtual void BeginPlay() override;
	
/*
 *	----------- Binded by Delegate
 */
	
/*
 *	----------- Other Method
 */
protected:
	void	CreatePlayerWidget();
	void	AddDefaultWidgetToViewport() const;
	void	RemoveDefaultWidgetFromViewport() const;
	
/*
 *	----------- Access
 */
public:
	TArray<TSubclassOf<URaidBossSkillBase>>		GetSkillClasses() const;
	TSubclassOf<UGameplayEffect>				GetCharacterStatusEffect() const;

	const FRaidBossInputAction&	GetInputAction() const;
	
	URaidBossUserWidget*	GetPlayerStateWidget() const;
	URaidBossUserWidget*	GetCrossHairWidget() const;
	URaidBossUserWidget*	GetQuickSlotWidget() const;
	URaidBossUserWidget*	GetSkillWidget() const;
	URaidBossUserWidget*	GetEquipWidget() const;

	UFUNCTION(BlueprintCallable)
	void	ToggleInventoryWidget() const;
	UFUNCTION(BlueprintCallable)
	void	ToggleSkillWidget() const;
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<URaidBossInventorySystem>	InventorySystem;
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<URaidBossSkillSystem>		SkillSystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<TSubclassOf<URaidBossSkillBase>>		SkillClasses;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect>				CharacterStatusEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InputAction")
	FRaidBossInputAction	InputActions;

	
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
    TObjectPtr<URaidBossUserWidget>	PlayerStateWidget;
    UPROPERTY(BlueprintReadOnly, Category = "Widget")
    TObjectPtr<URaidBossUserWidget>	CrossHairWidget;
    UPROPERTY(BlueprintReadOnly, Category = "Widget")
    TObjectPtr<URaidBossUserWidget>	QuickSlotWidget;
    UPROPERTY(BlueprintReadOnly, Category = "Widget")
    TObjectPtr<URaidBossUserWidget>	SkillWidget;
    UPROPERTY(BlueprintReadOnly, Category = "Widget")
    TObjectPtr<URaidBossUserWidget>	EquipWidget;
private:
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<URaidBossUserWidget>	PlayerStateWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<URaidBossUserWidget>	CrossHairWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<URaidBossUserWidget>	QuickSlotWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<URaidBossUserWidget>	SkillWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<URaidBossUserWidget>	EquipWidgetClass;
};
