#include "Enermy/RaidBossEnermyControllerBase.h"
#include "Enermy/RaidBossEnermyBase.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"

ARaidBossEnermyControllerBase::ARaidBossEnermyControllerBase()
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
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARaidBossEnermyControllerBase::OnTargetDetectedDelegated);
}

void ARaidBossEnermyControllerBase::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	PawnOwner = Cast<ARaidBossEnermyBase>(GetPawn());

	if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviourTree.Get()->BlackboardAsset.Get());
	}
}

void ARaidBossEnermyControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BehaviourTree.Get()) && IsValid(BehaviourTreeComponent))
	{
		RunBehaviorTree(BehaviourTree.Get());

		BehaviourTreeComponent->StartTree(*BehaviourTree.Get());
	}
}

void ARaidBossEnermyControllerBase::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void ARaidBossEnermyControllerBase::UpdateWalkSpeed(float speed)
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = speed;
}

void ARaidBossEnermyControllerBase::OnTargetDetectedDelegated(AActor* Actor, FAIStimulus Stimulus)
{
	if (IsValid(Actor) == false || IsValid(Blackboard) == false)
		return;
	if (Actor->ActorHasTag(FName("Enermy")))
		return;

	if (Actor->ActorHasTag(FName("Player")) && Stimulus.WasSuccessfullySensed())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Blackboard->SetValueAsBool(BBKey::HAS_LINE_OF_SIGHT, true);
		Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, Actor);
	}
	else if (Stimulus.WasSuccessfullySensed() == false && PawnOwner->GetDistanceTo(Actor) > 300.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARaidBossEnermyControllerBase::StopChasePlayer,
			3.f, false);
	}
}

void ARaidBossEnermyControllerBase::StopChasePlayer()
{
	Blackboard->SetValueAsBool(BBKey::HAS_LINE_OF_SIGHT, false);
	Blackboard->SetValueAsObject(BBKey::TARGET_ACTOR, nullptr);
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ARaidBossEnermyControllerBase::Attack(AActor* targetActor)
{
	if (IsValid(targetActor) == false || PawnOwner == false)
		return;

	PawnOwner->PlayAttackMontage();
	UGameplayStatics::ApplyDamage(targetActor, MeleeAttackDamage, this, nullptr, NULL);
}

UBehaviorTree* ARaidBossEnermyControllerBase::GetBehaviourTree() const
{
	return BehaviourTree;
}

UBehaviorTreeComponent* ARaidBossEnermyControllerBase::GetBehaviourTreeComponent() const
{
	return BehaviourTreeComponent;
}

UAISenseConfig_Sight* ARaidBossEnermyControllerBase::GetSight() const
{
	return Sight;
}

ARaidBossEnermyBase* ARaidBossEnermyControllerBase::GetPawnOwner() const
{
	return PawnOwner;
}
