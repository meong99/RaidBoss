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

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid Boss | PatrolLocation", meta = (AllowPrivateAccess = true))
    float SearchRadius = 2000.f;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
};
