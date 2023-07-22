#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "RaidBossSerathESkill.generated.h"

class ARaidBossEnemyBase;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class RAIDBOSS_API URaidBossSerathESkill : public URaidBossSkillBase
{
	GENERATED_BODY()

public:
	URaidBossSerathESkill();

	virtual void	ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void			EventReceivedCallback(const FGameplayEventData Payload);
	UFUNCTION()
	void			EndAbilityCallback();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Raid Boss")
		TArray<TObjectPtr<ARaidBossEnemyBase>>		EnermyArr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss")
		TObjectPtr<UParticleSystem>	Particle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Raid Boss")
		float	Range = 300;
};
