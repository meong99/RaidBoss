#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RaidBossAnimBase.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API URaidBossAnimBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	URaidBossAnimBase();

	UFUNCTION()
	virtual void NativeUpdateAnimation(float deltaTime) override;

	UAnimMontage*	GetDeathMontage() const { return DeathMonatage; }
	UAnimMontage*	GetAttackMontage() const;
	bool			IsInAir() const { return bIsInAir; }
	bool			IsAccelorating() const { return bIsAccelorating; }
	float			GetSpeed() const { return Speed; }
	float			GetRoll() const { return Roll; }
	float			GetPitch() const { return Pitch; }
	float			GetYaw() const { return Yaw; }
	float			GetYawDelta() const { return YawDelta; }
	FRotator		GetRotationLastTick() const { return RotationLastTick; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMonatage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage>	AttackMonatage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		bool		bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		bool		bIsAccelorating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		float		Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		float		Roll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		float		Pitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		float		Yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		float		YawDelta;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Update")
		FRotator	RotationLastTick;
};

