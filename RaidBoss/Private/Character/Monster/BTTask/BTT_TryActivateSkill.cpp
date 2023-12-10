// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/BTTask/BTT_TryActivateSkill.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RaidBossCharacterBase.h"

UBTT_TryActivateSkill::UBTT_TryActivateSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "TryActivateSkill";
	
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

void UBTT_TryActivateSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= MaximumWaitingTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UBTT_TryActivateSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARaidBossCharacterBase*		CharacterBase = Cast<ARaidBossCharacterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	UAbilitySystemComponent*	AbilitySystemComponent = CharacterBase ? CharacterBase->GetAbilitySystemComponent() : nullptr;
	UBlackboardComponent*		BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	
	if (CharacterBase == nullptr || AbilitySystemComponent == nullptr || BlackboardComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FGameplayEventData EventData;

	EventData.Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName));

	CharacterBase->SetIsMovementBlocked(true);

	int32 TriggerCount = AbilitySystemComponent->HandleGameplayEvent(TriggerTag, &EventData);

	if (TriggerCount < 1)
	{
		return EBTNodeResult::Failed;
	}

	if (bWaitAbilityEnded == false)
	{
		return EBTNodeResult::Succeeded;
	}
	
	DelegateHandle = AbilitySystemComponent->OnAbilityEnded.AddLambda([&](const FAbilityEndedData& EndedData) -> void
	{
		URaidBossAbilityBase* AbilityBase = Cast<URaidBossAbilityBase>(EndedData.AbilityThatEnded);
	
		if (AbilityBase && AbilityBase->GetAbilityTriggerTag() == TriggerTag)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	});
	
	bNotifyTick = true;
	
	return EBTNodeResult::InProgress; 
}

EBTNodeResult::Type UBTT_TryActivateSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ElapsedTime >= MaximumWaitingTime)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_TryActivateSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                           EBTNodeResult::Type TaskResult)
{
	ARaidBossCharacterBase*		CharacterBase = Cast<ARaidBossCharacterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	UAbilitySystemComponent*	AbilitySystemComponent = CharacterBase ? CharacterBase->GetAbilitySystemComponent() : nullptr;

	if (CharacterBase)
	{
		CharacterBase->SetIsMovementBlocked(false);
	}
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnAbilityEnded.Remove(DelegateHandle);
	}
	bNotifyTick = false;
	ElapsedTime = 0;
	DelegateHandle.Reset();
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
