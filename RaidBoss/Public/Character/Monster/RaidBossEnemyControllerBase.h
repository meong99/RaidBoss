#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "RaidBossEnemyControllerBase.generated.h"

struct FActorPerceptionUpdateInfo;
class AMonsterBase;
class ARaidBossCharacterBase;

namespace BBKey
{
	TCHAR const* const TARGET_ACTOR = TEXT("TargetActor");
	TCHAR const* const FOUND_PLAYER = TEXT("FoundPlayer");
	TCHAR const* const DESTINATION = TEXT("Destination");
	TCHAR const* const ACCEPTABLE_RADIUS = TEXT("AcceptableRadius");
	TCHAR const* const CAN_MOVE = TEXT("CanMove");
	TCHAR const* const SELF_ACTOR = TEXT("SelfActor");
	TCHAR const* const RETURN_TO_SPAWNER = TEXT("ReturnToSpawner");
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

	virtual void	Tick(float DeltaSeconds) override;
	
protected:
	virtual	void	OnPossess(APawn* InPawn) override;
	
	virtual	void	BeginPlay() override;

public:
	void	StopChasePlayer();

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Controller")
	AMonsterBase*	GetControlledCharacter() const { return ControlledMonster.Get(); }
	
protected:
	UFUNCTION()
	void	OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus);
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	TWeakObjectPtr<AMonsterBase>		ControlledMonster;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller")
	TObjectPtr<UBehaviorTree>			BehaviourTree;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller")
	TObjectPtr<UBehaviorTreeComponent>	BehaviourTreeComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Enermy Controller")
	TObjectPtr<UAISenseConfig_Sight>	Sight;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	float	TimeSinceOutChasingRange = 0;

};
