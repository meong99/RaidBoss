#pragma once

#include "CoreMinimal.h"
#include "UI/RaidBossUserWidgetBase.h"
#include "RaidBossSkillWidget.generated.h"

class URaidBossSkillSystem;
class UVerticalBox;
class UScrollBox;
class URaidBossSlotWidget;
class URaidBossSkillBase;

UCLASS()
class RAIDBOSS_API URaidBossSkillWidget : public URaidBossUserWidgetBase
{
	GENERATED_BODY()

public:
	void	InitializeSkillWidget(IN URaidBossSkillSystem* InWeakSkillSystem);
	void	UseSkill(int32 Index) const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	int32						GetSkillLevel(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	const URaidBossSkillBase*	GetSkillInstance(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	const URaidBossSkillBase*	GetSkillCDO(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	int32						GetCurrentSkillPoint() const;
	
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	int32	IncreaseSkillLevel(int32 Index);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	int32	DecreaseSkillLevel(int32 Index);

private:
	void	CreateSlots();

private:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget", meta=(AllowPrivateAccess))
	TWeakObjectPtr<URaidBossSkillSystem>	WeakSkillSystem;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget", meta=(AllowPrivateAccess, ExposeOnSpawn = true))
	TSubclassOf<URaidBossSlotWidget>		SlotClass;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget", meta=(AllowPrivateAccess))
	TArray<URaidBossSlotWidget*>			SkillSlots;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Skill Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox>		ScrollBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Skill Widget", meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox>	SkillVerticalBox;
};
