#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RaidBossSkillSystem.generated.h"

class URaidBossSkillWidget;
class URaidBossSkillBase;

UCLASS()
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
	void	ToggleSkillWidget();
/*
*	----------- Access(Get, Set, Check)
*/
public:
	void	SetSkillWidget(IN URaidBossSkillWidget* InSkillWidget);
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<TSubclassOf<URaidBossSkillBase>>	SkillClasses;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<URaidBossSkillWidget>		SkillWidget;
};
