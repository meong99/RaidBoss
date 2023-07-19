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
	
/*
 *	----------- Overrided
 */
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

/*
 *	----------- Access
 */
public:
	float	GetSearchRadius() const;
	
/*
 *	----------- Member Variables
 */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		float	SearchRadius = 2000.f;
};
