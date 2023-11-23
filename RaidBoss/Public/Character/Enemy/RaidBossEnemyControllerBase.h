#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RaidBossEnemyControllerBase.generated.h"

class ARaidBossCharacterBase;

namespace BBKey
{
	TCHAR const* const TARGET_ACTOR = TEXT("EnermyActor");
	TCHAR const* const HAS_LINE_OF_SIGHT = TEXT("HasLineOfSight");
	TCHAR const* const PATROL_LOCATION = TEXT("PatrolLocation");
}

namespace FilePath
{
	TCHAR const* const BehaviourTreePath = TEXT("/Script/AIModule.BehaviorTree'/Game/Character/Enermy/BT_DefaultEnermy.BT_DefaultEnermy'");
}

class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class ARaidBossEnemyBase;
class UGameplayEffect;
class URaidBossSkillBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	ARaidBossEnemyControllerBase();

	UFUNCTION()
	void	OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus);
	UFUNCTION()
	void	StopChasePlayer();
	void	UpdateWalkSpeed(float Speed);

	ARaidBossCharacterBase*	GetControlledCharacter() const;
	
protected:
	virtual	void	OnPossess(APawn* InPawn) override;
	virtual	void	BeginPlay() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree>			BehaviourTree;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent>	BehaviourTreeComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAISenseConfig_Sight>	Sight;

	FTimerHandle	TimerHandle;
};
