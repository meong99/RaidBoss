#pragma once

#include "CoreMinimal.h"
#include "Character/RaidBossCharacterBase.h"
#include "RaidBossPlayerBase.generated.h"

class ARaidBossPlayerControllerBase;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()

public:
	ARaidBossPlayerBase();

	virtual void	BeginPlay() override;

	void			MoveCharacter(const FVector2D& Value);
	void			LookCharacter(const FVector2D& Value);
	virtual void	JumpCharacter();
	virtual void	StopJumpCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Player Base")
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerController() const;
	UCameraComponent*					GetFollowCamera() const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Player Base", meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent>		CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Player Base", meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent>		FollowCamera;
};
