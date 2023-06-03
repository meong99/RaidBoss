#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shared/RaidBossSharedCharacter.h"
#include "RaidBossEnermyBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class URaidBossEnermyAnimBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossEnermyBase : public ARaidBossSharedCharacter
{
	GENERATED_BODY()
public:
	ARaidBossEnermyBase();

protected:
	virtual void	BeginPlay() override;

public:
	virtual float	TakeDamage(float DamageAmount,
							   struct FDamageEvent const& DamageEvent,
							   class AController* EventInstigator,
							   AActor* DamageCauser) override;
	virtual void	PlayDeathMontage();
	virtual void	PlayAttackMontage();

	/*
	 *	Access
	 */
public:
	FOnAttackEndDelegate	AttackDelegate;

protected:
	UPROPERTY()
	TObjectPtr<URaidBossEnermyAnimBase>	AnimInstance;
};
