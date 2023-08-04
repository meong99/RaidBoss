#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RaidBossRewardSystem.generated.h"

class ARaidBossPlayerControllerBase;
class URaidBossItemBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RAIDBOSS_API URaidBossRewardSystem : public UActorComponent
{
	GENERATED_BODY()
public:

	URaidBossRewardSystem();

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Reward System")
	void	NewReward(TArray<TSubclassOf<URaidBossItemBase>> Items, int32 Gold);

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Reward System")
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerControllerBase() const;

};
