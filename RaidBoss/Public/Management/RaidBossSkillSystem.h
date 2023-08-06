#pragma once

#include "CoreMinimal.h"
#include "RaidBossSkillSystem.generated.h"

class URaidBossSkillWidget;
class URaidBossSkillBase;
class ARaidBossPlayerControllerBase;
class URaidBossAbilitySystemComponent;

UCLASS(BlueprintType, Blueprintable)
class RAIDBOSS_API URaidBossSkillSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill System")
	void	InitialzeSkillSystem(URaidBossSkillWidget* InSkillWidget);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill System")
	void	ToggleSkillWidget() const;
	void	UseSkill(int32 Index) const;
	
	const URaidBossSkillBase*	GetSkillCDO(int32 Index) const;
	URaidBossSkillBase*			GetSkillInstance(int32 Index) const;
	int32						GetSkillCount() const;
	int32						GetCurrentSkillPoint() const;
	int32						GetSkillLevel(int32 Index) const;
	
	int32	IncreaseSkillLevel(int32 Index);
	int32	DecreaseSkillLevel(int32 Index);

private:
	void	GiveSkillToAbilityComponent() const;
	
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerControllerBase() const;
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Skill System", meta=(AllowPrivateAccess))
	TArray<TSubclassOf<URaidBossSkillBase>>		SkillClasses;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill System", meta=(AllowPrivateAccess))
	TObjectPtr<URaidBossSkillWidget>			SkillWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Skill System", meta=(AllowPrivateAccess))
	int32	CurrentSkillPoint = 5;
};
