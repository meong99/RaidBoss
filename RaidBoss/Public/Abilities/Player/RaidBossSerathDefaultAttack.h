#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Util/RaidBossTickableGameObject.h"
#include "RaidBossSerathDefaultAttack.generated.h"

class ARaidBossEnemyBase;
class URaidBossComboSystem;
class UGameplayEffect;

UCLASS()
class RAIDBOSS_API URaidBossSerathDefaultAttack : public URaidBossSkillBase, public FRaidBossTickableGameObject
{
	GENERATED_BODY()
	
public:
	URaidBossSerathDefaultAttack();
	
	virtual bool	CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void	PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
								const FGameplayEventData* TriggerEventData) override;
	virtual void	Tick(float DeltaTime) override;

protected:
	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
private:
	UFUNCTION()
	void	EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void	EndAbilityCallback();
	UFUNCTION()
	void	Interrupt();

	bool							ActivateTasks();
	void							InterpolateAttackDirection(float DeltaTime);
	TArray<ARaidBossEnemyBase*>		SelectTargets();
	void							ApplyEffectsToTargets(const TArray<ARaidBossEnemyBase*>& TargetArr);
	void							ResetAbility();
	float							CalculateAdditionalAttackPower();

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent>	WaitGameplayEvent;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait>	PlayMontageAndWait;
	UPROPERTY()
	TObjectPtr<URaidBossComboSystem>	Combo;
	bool	bIsRetriggered = false;
	float	IncreaseRate = 20;
};
