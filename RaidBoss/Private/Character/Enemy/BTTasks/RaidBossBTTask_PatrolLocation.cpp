#include "Character/Enemy/BTTasks/RaidBossBTTask_PatrolLocation.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

URaidBossBTTask_PatrolLocation::URaidBossBTTask_PatrolLocation()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type URaidBossBTTask_PatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	FNavLocation	location;
	
	if (navSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, location))
	{
		blackboard->SetValueAsVector(BBKey::PATROL_LOCATION, location.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

float URaidBossBTTask_PatrolLocation::GetSearchRadius() const
{
	return SearchRadius;
}
