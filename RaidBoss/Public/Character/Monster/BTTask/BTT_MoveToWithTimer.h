// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTT_MoveToWithTimer.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UBTT_MoveToWithTimer : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTT_MoveToWithTimer();
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss | BTTask")
	float	MaximumTime = 5;
	
	float ElapsedTime = 0;
};
