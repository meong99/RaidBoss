#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ARaidBossEnemyControllerBase::ARaidBossEnemyControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(FilePath::BehaviourTreePath);

	if (BTObject.Succeeded())
	{
		BehaviourTree = BTObject.Object;
	}

	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	
	if (IsValid(Sight))
	{
		Sight->SightRadius = 3000;
		Sight->LoseSightRadius = 3500;
		Sight->PeripheralVisionAngleDegrees = 180.0f;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		
		PerceptionComponent->ConfigureSense(*Sight);
	}
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARaidBossEnemyControllerBase::OnTargetDetectedDelegated);
}

void ARaidBossEnemyControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ControlledMonster.IsValid() == false)
	{
		return;
	}
	
	float bIsNotInChasingRange = ControlledMonster->GetDistanceBetweenSpawner() > ControlledMonster->GetMaxChasingDistance();

	if (bIsNotInChasingRange)
	{
		TimeSinceOutChasingRange += DeltaSeconds;
	}

	if (TimeSinceOutChasingRange >= ControlledMonster->GetTimerForStopChase())
	{
		StopChasePlayer();
	}
}

void ARaidBossEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledMonster = Cast<AMonsterBase>(InPawn);
	
	if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviourTree.Get()->BlackboardAsset.Get());
		Blackboard->SetValueAsObject(BBKey::SELF_ACTOR, InPawn);
	}
}

void ARaidBossEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BehaviourTree.Get()) && IsValid(BehaviourTreeComponent))
	{
		RunBehaviorTree(BehaviourTree.Get());

		BehaviourTreeComponent->StartTree(*BehaviourTree.Get());
	}
}

void ARaidBossEnemyControllerBase::OnTargetDetectedDelegated(AActor* Actor, FAIStimulus Stimulus)
{
	if (IsValid(Actor) == false || IsValid(Blackboard) == false || ControlledMonster.IsValid() == false)
	{
		PerceptionComponent->ForgetActor(Actor);
		return;
	}

	bool bIsNotPlayer = Cast<AMonsterBase>(Actor) != nullptr;
	bool bIsNotInChasingRange = ControlledMonster->GetDistanceBetweenSpawner() > ControlledMonster->GetMaxChasingDistance();
	
	if (bIsNotPlayer || bIsNotInChasingRange)
	{
		PerceptionComponent->ForgetActor(Actor);
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		TimeSinceOutChasingRange = 0;
		Blackboard->SetValueAsBool(BBKey::FOUND_PLAYER, true);
		Blackboard->SetValueAsBool(BBKey::RETURN_TO_SPAWNER, false);
		Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, Actor);
		ControlledMonster->SetMonsterHealthBarVisibility(true);
	}
}

void ARaidBossEnemyControllerBase::StopChasePlayer()
{
	if (IsValid(Blackboard) == false || ControlledMonster.IsValid() == false)
	{
		return;
	}

	TimeSinceOutChasingRange = 0;
	
	ControlledMonster->SetMonsterHealthBarVisibility(false);
	
	PerceptionComponent->ForgetAll();
	
	Blackboard->SetValueAsBool(BBKey::RETURN_TO_SPAWNER, true);
	Blackboard->SetValueAsVector(BBKey::DESTINATION, ControlledMonster->GetSpawnerLocation());
	Blackboard->SetValueAsBool(BBKey::FOUND_PLAYER, false);
	Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, nullptr);
}

AMonsterBase* ARaidBossEnemyControllerBase::GetControlledCharacter() const
{
	return ControlledMonster.Get();
}
