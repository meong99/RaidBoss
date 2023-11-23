#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
		Sight->PeripheralVisionAngleDegrees = 150.0f;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;

		PerceptionComponent->ConfigureSense(*Sight);
	}

	// PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARaidBossEnemyControllerBase::OnTargetDetectedDelegated);
}

void ARaidBossEnemyControllerBase::OnTargetDetectedDelegated(AActor* Actor, FAIStimulus Stimulus)
{
	ARaidBossCharacterBase* ControlledCharacter = GetControlledCharacter();
	
	if (IsValid(Actor) == false || IsValid(Blackboard) == false || IsValid(ControlledCharacter) == false)
	{
		return;
	}
	if (Actor->ActorHasTag(FName("Enemy")))
	{
		return;
	}

	if (Actor->ActorHasTag(FName("Player")) && Stimulus.WasSuccessfullySensed())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Blackboard->SetValueAsBool(BBKey::HAS_LINE_OF_SIGHT, true);
		Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, Actor);
	}
	else if (Stimulus.WasSuccessfullySensed() == false && ControlledCharacter->GetDistanceTo(Actor) > 300.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARaidBossEnemyControllerBase::StopChasePlayer,
			3.f, false);
	}
}

void ARaidBossEnemyControllerBase::StopChasePlayer()
{
	if (IsValid(Blackboard) == false)
	{
		return;
	}

	Blackboard->SetValueAsBool(BBKey::HAS_LINE_OF_SIGHT, false);
	Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, nullptr);
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ARaidBossEnemyControllerBase::UpdateWalkSpeed(float Speed)
{
	ARaidBossCharacterBase* ControlledCharacter = GetControlledCharacter();
	
	if (IsValid(ControlledCharacter) == false)
		return;

	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

ARaidBossCharacterBase* ARaidBossEnemyControllerBase::GetControlledCharacter() const
{
	return Cast<ARaidBossCharacterBase>(GetCharacter());
}

void ARaidBossEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviourTree.Get()->BlackboardAsset.Get());
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
	
	ACharacter*	CurrentPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (CurrentPlayer)
	{
		Blackboard->SetValueAsBool(BBKey::HAS_LINE_OF_SIGHT, true);
		Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, CurrentPlayer);
	}
}
