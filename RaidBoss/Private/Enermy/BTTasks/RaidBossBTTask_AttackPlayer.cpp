#include "Enermy/BTTasks/RaidBossBTTask_AttackPlayer.h"
#include "Enermy/RaidBossEnermyBase.h"
#include "Enermy/RaidBossEnermyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

URaidBossBTTask_AttackPlayer::URaidBossBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");
	bNotifyTick = true;
}

EBTNodeResult::Type URaidBossBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	ARaidBossEnermyBase*				OwnerCharacter = Cast<ARaidBossEnermyBase>(OwnerComp.GetAIOwner()->GetCharacter());
	URaidBossAbilitySystemComponent*	AbilityCom = OwnerCharacter->GetRaidBossAbilitySystemComponent();

	if (IsValid(OwnerCharacter) && IsValid(AbilityCom))
	{
		if (AbilityCom->TryActivateAbility(OwnerCharacter->GetHandleOfDefaultAbilities(ERaidBossAbilityInputID::Attack), false) == false)
			return EBTNodeResult::Failed;
		AbilityCom->OnAbilityEnded.AddLambda([&] (const FAbilityEndedData& AbilityEndedData)-> void
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
