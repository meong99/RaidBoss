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

	virtual void	BeginPlay() final;

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	OpenStoreWidget();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	CloseStoreWidget();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	bool	RespondBuyingRequest(int32 ProductsIndex);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	bool	RespondSellingRequest(EITemCategory Category, int32 InventoryIndex);

	const TArray<TSubclassOf<URaidBossItemBase>>&	GetProducts() const;
	const int32&									GetGoldFromPlayer() const;
	const TArray<FItemInformation>&					GetEquipItemsFromPlayer() const;
	const TArray<FItemInformation>&					GetConsumableItemsFromPlayer() const;
	int32											GetMaximumItemAmount() const;

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Store System")
	void	SetPlayerController(ARaidBossPlayerControllerBase* InPlayerController);

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | Store System", meta=(AllowPrivateAccess))
	TObjectPtr<URaidBossStoreWidget>		StoreWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | Store System", meta=(AllowPrivateAccess))
	TArray<TSubclassOf<URaidBossItemBase>>	Products;

	TObjectPtr<ARaidBossPlayerControllerBase>	PlayerController;
	TWeakObjectPtr<URaidBossInventorySystem>	InventorySystem;
};
