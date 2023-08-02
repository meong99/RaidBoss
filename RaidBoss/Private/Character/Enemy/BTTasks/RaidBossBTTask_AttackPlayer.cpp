#include "Character/Enemy/BTTasks/RaidBossBTTask_AttackPlayer.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Global/RaidBoss.h"

URaidBossBTTask_AttackPlayer::URaidBossBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type URaidBossBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARaidBossEnemyBase*	OwnerCharacter = Cast<ARaidBossEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter());
	if (IsValid(OwnerCharacter) == false)
		return EBTNodeResult::Failed;

	URaidBossAbilitySystemComponent*	AbilityCom = OwnerCharacter->GetRaidBossAbilitySystemComponent();
	if (IsValid(AbilityCom) == false)
		return EBTNodeResult::Failed;

	AbilityCom->PressInputID(static_cast<int32>(ERaidBossAbilityInputID::DefaultAttack));
	AbilityCom->OnAbilityEnded.AddLambda([&] (const FAbilityEndedData& AbilityEndedData)-> void
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	return EBTNodeResult::InProgress;
}
