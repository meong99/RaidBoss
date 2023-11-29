#include "Character/Enemy/BTTasks/RaidBossBTTask_AttackPlayer.h"

#include "AbilitySystemComponent.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/RaidBossCharacterBase.h"

URaidBossBTTask_AttackPlayer::URaidBossBTTask_AttackPlayer()
{
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type URaidBossBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARaidBossCharacterBase*	OwnerCharacter = Cast<ARaidBossCharacterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	
	if (OwnerCharacter)
	{
		OwnerCharacter->Attack();
	}
	
	return EBTNodeResult::Succeeded;
}
