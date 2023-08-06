#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RaidBossPlayerControllerBase.generated.h"

class URaidBossRewardSystem;
class ARaidBossPlayerBase;
class URaidBossInventorySystem;
class URaidBossSkillSystem;
class URaidBossAbilityBase;
class URaidBossAbilitySystemComponent;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	ARaidBossPlayerControllerBase();

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	MoveCharacter(FVector2D Value) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	LookCharacter(FVector2D Value) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	JumpCharacter() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	StopJumpCharacter() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	ToggleInventoryWidget() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	ToggleSkillWidget() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	void	AttemptDropItem(EITemCategory ItemCategory, int32 Index);

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	ARaidBossPlayerBase*				GetRaidBossPlayerBase() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Player Controller")
	URaidBossInventorySystem*			GetInventorySystem() const;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Player Controller")
	TObjectPtr<URaidBossInventorySystem>	InventorySystem;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Player Controller")
	TObjectPtr<URaidBossSkillSystem>		SkillSystem;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Player Controller")
	TObjectPtr<URaidBossRewardSystem>		RewardSystem;
	
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Player Controller")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultAbilities;
};
