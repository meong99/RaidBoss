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
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Anim Base")
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

	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		bool		bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		bool		bIsAccelorating;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		bool		bIsFocusing;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		MoveForward;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		MoveRight;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		Roll;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		Pitch;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Anim Base")
		float		Yaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Anim Base")
	TObjectPtr<UAnimMontage>	DeathMontage;
};

