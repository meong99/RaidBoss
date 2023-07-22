#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaidBossEquipmentWidget.generated.h"

class URaidBossSlotWidget;
class URaidBossInventorySystem;

UCLASS()
class RAIDBOSS_API URaidBossEquipmentWidget : public UUserWidget
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
/*
*	----------- Access(Get, Set, Check)
*/
	void	SetWeakInventorySystem(IN URaidBossInventorySystem* InWeakInventorySystem);
/*
*	----------- Member Variables
*/

protected:
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Equipment Widget")
	TWeakObjectPtr<URaidBossInventorySystem>	WeakInventorySystem;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Equipment Widget", meta = (ExposeOnSpawn = true))
	TSubclassOf<URaidBossSlotWidget>			SlotClass;
};
