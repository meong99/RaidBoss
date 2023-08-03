#pragma once

#include "CoreMinimal.h"
#include "Character/RaidBossCharacterBase.h"
#include "RaidBossPlayerBase.generated.h"

class URaidBossEquipmentItem;
class URaidBossItemBase;
class USpringArmComponent;
class UCameraComponent;
class ARaidBossPlayerControllerBase;
class UUserWidget;
class ARaidBossInteractionBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossPlayerBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()

public:
	ARaidBossPlayerBase();
/*
 *	----------- Overrided
 */
protected:
	virtual void	BeginPlay() override;

/*
 *	----------- Other Method
 */
/*
 *	----------- Binded by Delegate
 */
public:
	void			MoveCharacter(const FVector2D& Value);
	void			LookCharacter(const FVector2D& Value);
	virtual void	JumpCharacter();
	virtual void	StopJumpCharacter();
	void			Interaction();
	
/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Player Base")
	TArray<ARaidBossInteractionBase*>	GetInteractionableActors() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Player Base")
	ARaidBossPlayerControllerBase*		GetRaidBossPlayerController() const;
	float								GetGold() const;
	
/*
 *	----------- Member Variables
 */
public:
	UPROPERTY(VisibleAnywhere, Category = "Raid Boss | Player Base")
	TObjectPtr<USpringArmComponent>		CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Raid Boss | Player Base")
	TObjectPtr<UCameraComponent>		FollowCamera;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Player Base")
	TArray<URaidBossSkillBase*>	SkillObjects;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Player Base")
	float						Gold = 0;
	
	UPROPERTY()
	TArray<ARaidBossInteractionBase*>	InteractionableActors;
};
