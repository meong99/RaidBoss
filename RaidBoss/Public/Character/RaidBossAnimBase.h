#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RaidBossAnimBase.generated.h"

class ARaidBossCharacterBase;

UCLASS(Abstract)
class RAIDBOSS_API URaidBossAnimBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	URaidBossAnimBase();
/*
 *	----------- Overrided
 */
public:
	UFUNCTION()
	virtual void NativeUpdateAnimation(float deltaTime) override;
	virtual void NativeInitializeAnimation() override;
/*
 *	----------- Binded by Delegate
 */

	
/*
 *	----------- Other Method
 */
	
/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable)
	ARaidBossCharacterBase*	GetOwningCharacter() const;
	bool					IsInAir() const { return bIsInAir; }
	bool					IsAccelorating() const { return bIsAccelorating; }
	bool					GetIsFocusing() const { return bIsFocusing; }
	float					GetSpeed() const { return Speed; }
	float					GetMoveForward() const { return MoveForward; }
	float					GetMoveRight() const { return MoveRight; }

	void	SetIsFocusing(bool IsFocusing) { bIsFocusing = IsFocusing; }
	void	SetMoveForward(float Forward) { MoveForward = Forward; }
	void	SetMoveRight(float Right) { MoveRight = Right; }

/*
 *	----------- Member Variables
 */
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Update")
		bool		bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		bool		bIsAccelorating;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		bool		bIsFocusing;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		MoveForward;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		MoveRight;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		Roll;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		Pitch;
	UPROPERTY(BlueprintReadOnly, Category = "Update")
		float		Yaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>	DeathMontage;
};

