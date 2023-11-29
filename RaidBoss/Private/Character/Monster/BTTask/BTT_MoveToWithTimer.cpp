// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/BTTask/BTT_MoveToWithTimer.h"

UBTT_MoveToWithTimer::UBTT_MoveToWithTimer()
{
	NodeName = "Move to target with timer";
}

void UBTT_MoveToWithTimer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= MaximumTime)
	{
		ElapsedTime = 0;
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UBTT_MoveToWithTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return Result;
}

void UBTT_MoveToWithTimer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                          EBTNodeResult::Type TaskResult)
{
	ElapsedTime = 0;
	bNotifyTick = false;
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
