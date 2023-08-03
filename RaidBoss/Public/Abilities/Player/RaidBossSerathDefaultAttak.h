#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Util/RaidBossTickableGameObject.h"
#include "RaidBossSerathDefaultAttak.generated.h"

class ARaidBossEnemyBase;
class URaidBossComboSystem;
class UGameplayEffect;

UCLASS()
class RAIDBOSS_API URaidBossSerathDefaultAttak : public URaidBossSkillBase, public FRaidBossTickableGameObject
{
	GENERATED_BODY()
	
public:
	URaidBossSerathDefaultAttak();
	
/*
 *	----------- Overrided
 */
public:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void	PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
								const FGameplayEventData* TriggerEventData) override;
	virtual void	Tick(float DeltaTime) override;
/*
 *	----------- Binded by Delegate
 */
public:
	UFUNCTION()
	void	EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void	EndAbilityCallback();
	UFUNCTION()
	void	Interrupt();

/*
 *	----------- Other Method
 */
protected:
	bool							ActivateTasks();
	void							InterpolateAttackDirection(float DeltaTime);
	TArray<ARaidBossEnemyBase*>		SelectTargets();
	void							ApplyEffecsToTargets(const TArray<ARaidBossEnemyBase*>& TargetArr);
	void							ResetAbility();
	float							CalculateAdditialnalAttackPower();
/*
 *	----------- Access
 */
	
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
	UPROPERTY()
	TObjectPtr<URaidBossComboSystem>	Combo;
	bool	bIsRetrrigered = false;
	float	IncreaseRate = 20;
};
