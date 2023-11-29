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

	virtual void	JumpCharacter();
	virtual void	StopJumpCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Player Base")
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerController() const;
};
