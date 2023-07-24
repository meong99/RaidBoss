#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RaidBossSkillSystem.generated.h"

class URaidBossSkillWidget;
class URaidBossSkillBase;
class ARaidBossPlayerControllerBase;
class URaidBossAbilitySystemComponent;

UCLASS(BlueprintType, Blueprintable)
class RAIDBOSS_API URaidBossSkillSystem : public UObject
{
	GENERATED_BODY()
	
/*
*	----------- Override
*/
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill System")
	void	InitialzeSkillSystem(URaidBossSkillWidget* InSkillWidget);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill System")
	void	ToggleSkillWidget() const;
	
protected:
	void	GiveSkillToAbilityComponent();
/*
*	----------- Access(Get, Set, Check)
*/
public:
	const URaidBossSkillBase*				GetSkillObject(int32 Index) const;
	int32									GetSkillAmount() const;
	int32									GetCurrentSkillPoint() const;

private:
	ARaidBossPlayerControllerBase*			GetRaidBossPlayerControllerBase() const;
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill System", meta=(ExposeOnSpawn=true))
	TObjectPtr<URaidBossAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Skill System")
	TArray<TSubclassOf<URaidBossSkillBase>>		SkillClasses;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill System")
	TObjectPtr<URaidBossSkillWidget>			SkillWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Skill System")
	int32	CurrentSkillPoint = 5;
};
