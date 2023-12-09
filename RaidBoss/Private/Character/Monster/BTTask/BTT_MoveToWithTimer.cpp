// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/BTTask/BTT_MoveToWithTimer.h"
#include "AIController.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Management/RaidBossGameplayTags.h"

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

	ARaidBossCharacterBase*				CharacterBase = Cast<ARaidBossCharacterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	URaidBossAbilitySystemComponent*	AbilitySystemComponent = CharacterBase ? CharacterBase->GetRaidBossAbilitySystemComponent() : nullptr;
	UBlackboardComponent*				BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	
	if (CharacterBase == nullptr || AbilitySystemComponent == nullptr || BlackboardComponent == nullptr)
	{
		AcceptableRadius = 5;
	}
	else
	{
		URaidBossSkillBase*	BasicAttack = Cast<URaidBossSkillBase>(
			AbilitySystemComponent->GetInstanceAbilitiesByTag().FindRef(RaidBossGameplayTags::Get().Attack_BasicAttack));

		if (BasicAttack)
		{
			AcceptableRadius = BasicAttack->GetSkillRange() * 0.55f;
		}
		else
		{
			AcceptableRadius = 5;
		}
	}
	
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
