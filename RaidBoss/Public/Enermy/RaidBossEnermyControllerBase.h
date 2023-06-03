#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RaidBossEnermyControllerBase.generated.h"

namespace BBKey
{
	TCHAR const* const TARGET_ACTOR = TEXT("EnermyActor");
	TCHAR const* const HAS_LINE_OF_SIGHT = TEXT("HasLineOfSight");
	TCHAR const* const PATROL_LACATION = TEXT("PatrolLocation");
}

namespace FilePath
{
	TCHAR const* const BehaviourTreePath = TEXT("/Script/AIModule.BehaviorTree'/Game/Enermy/BT_DefaultEnermy.BT_DefaultEnermy'");
}

class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class ARaidBossEnermyBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossEnermyControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	ARaidBossEnermyControllerBase();

protected:
	virtual	void	OnPossess(APawn* inPawn) override;
	virtual	void	BeginPlay() override;

public:
	virtual void	Tick(float deltaTime) override;

	UFUNCTION()
	virtual void	UpdateWalkSpeed(float speed);
	UFUNCTION()
	virtual	void	OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus);
	UFUNCTION()
	virtual void	StopChasePlayer();
	UFUNCTION()
	virtual	void	Attack(AActor* targetActor);

	/*
	 *	Access
	 */
	UFUNCTION(BlueprintCallable)
	UBehaviorTree*			GetBehaviourTree() const;
	UFUNCTION(BlueprintCallable)
	UBehaviorTreeComponent*	GetBehaviourTreeComponent() const;
	UFUNCTION(BlueprintCallable)
	UAISenseConfig_Sight*	GetSight() const;
	UFUNCTION(BlueprintCallable)
	ARaidBossEnermyBase*	GetPawnOwner() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree>			BehaviourTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent>	BehaviourTreeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAISenseConfig_Sight>	Sight;
	UPROPERTY()
	TObjectPtr<ARaidBossEnermyBase>		PawnOwner;

	float			MeleeAttackDamage = 50.f;
	FTimerHandle	TimerHandle;
};
