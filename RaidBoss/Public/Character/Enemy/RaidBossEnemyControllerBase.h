#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RaidBossEnemyControllerBase.generated.h"

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
class ARaidBossEnemyBase;
class UGameplayEffect;
class URaidBossAbilityBase;
class URaidBossSkillBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	ARaidBossEnemyControllerBase();

/*
 *	----------- Overrided
 */
protected:
	virtual	void	OnPossess(APawn* inPawn) override;
	virtual	void	BeginPlay() override;
	
/*
 *	----------- Binded by Delegate
 */
/*
 *	----------- Other Method
 */
public:
	UFUNCTION()
	virtual void	UpdateWalkSpeed(float speed);
	UFUNCTION()
	virtual	void	OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus);
	UFUNCTION()
	virtual void	StopChasePlayer();

/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable)
	ARaidBossEnemyBase*	GetControlledCharacter() const;
	
	TArray<TSubclassOf<URaidBossSkillBase>>		GetSkillClasses() const;
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree>			BehaviourTree;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent>	BehaviourTreeComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI Perception", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAISenseConfig_Sight>	Sight;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<URaidBossSkillBase>>		EnemySkillClasses;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect>				CharacterStatusEffect;
	
	FTimerHandle	TimerHandle;
};
