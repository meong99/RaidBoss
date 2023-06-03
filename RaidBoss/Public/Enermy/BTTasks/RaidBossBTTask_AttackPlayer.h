#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RaidBossBTTask_AttackPlayer.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossBTTask_AttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URaidBossBTTask_AttackPlayer();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
