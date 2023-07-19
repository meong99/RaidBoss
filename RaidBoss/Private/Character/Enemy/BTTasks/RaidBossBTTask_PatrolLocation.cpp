#include "Character/Enemy/BTTasks/RaidBossBTTask_PatrolLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"

URaidBossBTTask_PatrolLocation::URaidBossBTTask_PatrolLocation()
{
	NodeName = TEXT("Find Random Location");
}

float URaidBossBTTask_PatrolLocation::GetSearchRadius() const
{
	return SearchRadius;
}

EBTNodeResult::Type URaidBossBTTask_PatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation	location = {};
	ARaidBossEnemyControllerBase* AIController = Cast<ARaidBossEnemyControllerBase>(OwnerComp.GetAIOwner());

	if (IsValid(AIController) == false)
		return EBTNodeResult::Failed;

	UBlackboardComponent*	blackboard = AIController->GetBlackboardComponent();
	const APawn*			AIPawn = AIController->GetPawn();

	if (IsValid(AIPawn) == false || IsValid(blackboard) == false)
		return EBTNodeResult::Failed;

	const FVector				origin = AIPawn->GetActorLocation();
	const UNavigationSystemV1*	navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (IsValid(navSystem) == false)
		return EBTNodeResult::Failed;

	if (navSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, location))
	{
		blackboard->SetValueAsVector(BBKey::PATROL_LACATION, location.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
