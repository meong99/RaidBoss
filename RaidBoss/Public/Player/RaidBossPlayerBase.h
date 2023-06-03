#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shared/RaidBossSharedCharacter.h"
#include "RaidBossPlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerBase : public ARaidBossSharedCharacter
{
	GENERATED_BODY()

public:
	ARaidBossPlayerBase();
	
	UFUNCTION(BlueprintCallable)
	USpringArmComponent*	GetCameraBoom() const;
	UFUNCTION(BlueprintCallable)
	UCameraComponent*		GetFollowCamera() const;

protected:
	virtual void	SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void	OnRep_PlayerState() override;
	virtual void	MoveForward(float value);
	virtual void	MoveRight(float value);

	/*
	*	Properties
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USpringArmComponent>	CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCameraComponent>	FollowCamera;
};
