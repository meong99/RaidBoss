#pragma once

#include "CoreMinimal.h"
#include "Character/RaidBossCharacterBase.h"
#include "InputActionValue.h"
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
	virtual void	PossessedBy(AController* NewController) override;
	virtual void	SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

/*
 *	----------- Other Method
 */
protected:
	void	AddMappingSystem();
	void	BindInputAction(UEnhancedInputComponent* EnhancedInputComponent);
	void	CreateSkillObjects();
	// void	CreateItemObjects();
	void	ApplyCharacterStatusEffect();
	
/*
 *	----------- Binded by Delegate
 */
protected:
	virtual void	Jump() override;
	virtual void	StopJumping() override;
	UFUNCTION(BlueprintCallable)
	void			MoveCharacter(const FInputActionValue& Value);
	void			MoveForward(float Value);
	void			MoveRight(float Value);
	void			LookCharacter(const FInputActionValue& Value);
	// void			ToggleItemWidget(const FInputActionValue& Value);
	void			ToggleSkillWidget(const FInputActionValue& Value);
	void			ToggleEquipWidget(const FInputActionValue& Value);
	void			Interaction(const FInputActionValue& Value);
	void			ActiveAbilityByInput(const FInputActionValue& Value, ERaidBossAbilityInputID InputID);
	
/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable)
	TArray<ARaidBossInteractionBase*>	GetInteractionableActors() const;
	ARaidBossPlayerControllerBase*		GetRiadBossPlayerController() const;
	float								GetGold() const;
	
	const TMap<EEquipType, URaidBossEquipmentItem*>&	GetEquippedItems() const;

/*
 *	----------- Member Variables
 */
public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent>		CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent>		FollowCamera;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TArray<URaidBossSkillBase*>	SkillObjects;
	UPROPERTY(BlueprintReadOnly, Category="Belongings")
	float						Gold = 0;
	
	UPROPERTY()
	TArray<ARaidBossInteractionBase*>	InteractionableActors;
};
