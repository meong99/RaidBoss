#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RaidBossPlayerControllerBase.generated.h"

class URaidBossAbilitySystemComponent;
class ARaidBossPlayerBase;
class URaidBossInventorySystem;
class URaidBossSkillSystem;
class URaidBossAbilityBase;
class UGameplayEffect;
class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FRaidBossInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction")
	TObjectPtr<UInputAction>			DefaultAttackAction;
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
/*
 *	----------- Binded by Delegate
 */
	
/*
 *	----------- Other Method
 */
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	MoveCharacter(FVector2D Value) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	LookCharacter(FVector2D Value) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	JumpCharacter() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	StopJumpCharacter() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	Interaction();
/*
 *	----------- Access
 */
public:
	ARaidBossPlayerBase*				GetRaidBossPlayerBase() const;
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	TSubclassOf<UGameplayEffect>		GetCharacterStatusEffect() const;
	const FRaidBossInputAction&			GetInputAction() const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	ToggleInventoryWidget() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	ToggleSkillWidget() const;
	
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Player Controller")
	TObjectPtr<URaidBossInventorySystem>	InventorySystem;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Player Controller")
	TObjectPtr<URaidBossSkillSystem>		SkillSystem;
	
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Player Controller")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Player Controller")
	TSubclassOf<UGameplayEffect>				CharacterStatusEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Player Controller")
	FRaidBossInputAction	InputActions;
};
