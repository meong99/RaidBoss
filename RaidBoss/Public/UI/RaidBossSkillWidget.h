#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaidBossSkillWidget.generated.h"

class URaidBossSkillSystem;
class UVerticalBox;
class UScrollBox;
class URaidBossSlotWidget;
class URaidBossSkillBase;

UCLASS()
class RAIDBOSS_API URaidBossSkillWidget : public UUserWidget
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
	void	InitializeSkillWidget(IN URaidBossSkillSystem* InWeakSkillSystem);
protected:
	void					CreateSlots();
	URaidBossSlotWidget*	CreateNewSlot();
/*
*	----------- Access(Get, Set, Check)
*/
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	const URaidBossSkillBase*	GetSkillObject(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Skill Widget")
	int32						GetCurrentSkillPoint() const;
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget")
	TWeakObjectPtr<URaidBossSkillSystem>	WeakSkillSystem;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget")
	TArray<URaidBossSlotWidget*>			SkillSlots;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Skill Widget", meta = (ExposeOnSpawn = true))
	TSubclassOf<URaidBossSlotWidget>		SlotClass;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Skill Widget", meta=(BindWidget))
	TObjectPtr<UScrollBox>		ScrollBox;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Skill Widget", meta=(BindWidget))
	TObjectPtr<UVerticalBox>	SkillVerticalBox;
};
