#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Management/RaidBossInventorySystem.h"
#include "RaidBossStoreSystem.generated.h"

class URaidBossInventorySystem;
class ARaidBossPlayerControllerBase;
class URaidBossItemBase;
class URaidBossStoreWidget;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API URaidBossStoreSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	URaidBossStoreSystem();
/*
*	----------- Override
*/
	virtual void	BeginPlay() override;
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	OpenStoreWidget();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	CloseStoreWidget();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	bool	RespondBuyingRequest(int32 ProductsIndex);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	bool	RespondSellingRequest(EITemCategory Category, int32 InventoryIndex);
/*
*	----------- Access(Get, Set, Check)
*/
public:
	const TArray<TSubclassOf<URaidBossItemBase>>&	GetProducts() const;
	const int32&									GetGoldFromPlayer() const;
	const TArray<FItemInfomation>&					GetEquipItemsFromPlayer() const;
	const TArray<FItemInfomation>&					GetConsumableItemsFromPlayer() const;
	int32											GetMaximumItemAmount() const;

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	SetPlayerController(ARaidBossPlayerControllerBase* InPlayerController);
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | Store System", meta=(AllowPrivateAccess))
	TObjectPtr<URaidBossStoreWidget>		StoreWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | Store System", meta=(AllowPrivateAccess))
	TArray<TSubclassOf<URaidBossItemBase>>	Products;

	TObjectPtr<ARaidBossPlayerControllerBase>	PlayerController;
	TWeakObjectPtr<URaidBossInventorySystem>	InventorySystem;
};
