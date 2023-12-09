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
	virtual void NativeUpdateAnimation(float deltaTime) override;

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	ARaidBossCharacterBase*	GetOwningCharacter() const;
	bool					IsInAir() const { return bIsInAir; }
	bool					IsAccelerating() const { return bIsAccelerating; }
	bool					GetIsFocusing() const { return bIsFocusing; }
	float					GetSpeed() const { return Speed; }
	float					GetMoveForward() const { return MoveForward; }
	float					GetMoveRight() const { return MoveRight; }

	void	SetIsFocusing(bool bInIsFocusing) { bIsFocusing = bInIsFocusing; }
	void	SetMoveForward(float Forward) { MoveForward = Forward; }
	void	SetMoveRight(float Right) { MoveRight = Right; }

private:
	
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float InterpSpeed = 5;
	
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	//
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsFocusing;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		MoveForward;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		MoveRight;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		InputMoveForward;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		InputMoveRight;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		Roll;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		Pitch;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	float		Yaw;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsDuringAlign;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsTurnLeft = false;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base", meta=(AllowPrivateAccess))
	bool		bIsAttacking = false;
};

