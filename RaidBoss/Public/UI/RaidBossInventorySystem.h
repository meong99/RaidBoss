#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RaidBossInventorySystem.generated.h"

enum class EITemCategory : uint8;
class URaidBossItemBase;
class URaidBossConsumableItem;
class URaidBossEquipmentItem;
class URaidBossInventoryWidget;
class ARaidBossPlayerControllerBase;

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossInventorySystem : public UObject
{
	GENERATED_BODY()

public:
	URaidBossInventorySystem(const FObjectInitializer& Initializer);
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
	void	ChangeItemOrder(int32 Index1, int32 Index2, EITemCategory Category);
	void	SwapItems(TArray<URaidBossItemBase*>& Items, int32 Index1, int32 Index2);
	void	ToggleInventoryWidget();
	
	UFUNCTION(BlueprintCallable)
	void	AddNewItem(URaidBossItemBase* NewItem);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	ARaidBossPlayerControllerBase*	GetRaidBossPlayerControllerBase() const;
	TArray<URaidBossItemBase*>		GetEquipmentItems() const;
	TArray<URaidBossItemBase*>		GetConsumableItemObjects() const;
	
	int32	GetMaximumItemCount() const;

	UFUNCTION(BlueprintCallable)
	void	SetInventoryWidget(URaidBossInventoryWidget* InInventoryWidget);
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<URaidBossInventoryWidget>	InventoryWidget;
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TArray<URaidBossItemBase*>				EquipItemObjects;
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TArray<URaidBossItemBase*>				ConsumableItemObjects;
	UPROPERTY(BlueprintReadOnly)
	int32 MaximumItemCount = 30;
};
