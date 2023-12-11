// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryActivateSkill.generated.h"

struct FAbilityEndedData;

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UBTT_TryActivateSkill : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTT_TryActivateSkill(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void                TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void                OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                               EBTNodeResult::Type TaskResult) override;

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | BTT")
    FGameplayTag    TriggerTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | BTT")
    float           MaximumWaitingTime = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Boss | BTT")
    float           RotateSpeedForFaceTarget = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss | BTT")
    bool            bWaitAbilityEnded = true;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    float           ElapsedTime = 0;

    FDelegateHandle DelegateHandle;
};
