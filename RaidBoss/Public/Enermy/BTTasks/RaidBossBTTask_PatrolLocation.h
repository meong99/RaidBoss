#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RaidBossBTTask_PatrolLocation.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossBTTask_PatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URaidBossBTTask_PatrolLocation();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	float	GetSearchRadius() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		float	SearchRadius = 500.f;
};
