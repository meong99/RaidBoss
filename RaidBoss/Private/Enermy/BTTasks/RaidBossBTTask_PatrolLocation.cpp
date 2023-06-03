// Fill out your copyright notice in the Description page of Project Settings.


#include "Enermy/BTTasks/RaidBossBTTask_PatrolLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enermy/RaidBossEnermyControllerBase.h"

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
	ARaidBossEnermyControllerBase* AIController = Cast<ARaidBossEnermyControllerBase>(OwnerComp.GetAIOwner());

	if (!IsValid(AIController))
		return EBTNodeResult::Failed;

	UBlackboardComponent*	blackboard = AIController->GetBlackboardComponent();
	const APawn*			AIPawn = AIController->GetPawn();

	if (IsValid(AIPawn) == false || IsValid(blackboard) == false)
		return EBTNodeResult::Failed;

	const FVector				origin = AIPawn->GetActorLocation();
	const UNavigationSystemV1*	navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!IsValid(navSystem))
		return EBTNodeResult::Failed;

	if (IsValid(navSystem) && navSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, location))
	{
		blackboard->SetValueAsVector(BBKey::PATROL_LACATION, location.Location);
	}

	return EBTNodeResult::Succeeded;
}
