#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RaidBossCharacterBase.generated.h"

class	URaidBossSkillBase;
class	URaidBossAbilityBase;
class	URaidBossAbilitySystemComponent;
class	URaidBossCharacterStatusAttributeSet;
class	UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Alive = 1 UMETA(DisplayName = "Alive"),
	CanMove = 2 UMETA(DisplayName = "CanMove"),
	CanUsingAttack = 4 UMETA(DisplayName = "CanUsingAttack"),
};

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARaidBossCharacterBase();
	
/*
 *	----------- Overrided
 */
protected:
	virtual void	PossessedBy(AController* NewController) override;

/*
 *	----------- Binded by Delegate
 */
		
/*
 *	----------- Other Method
 */
public:

	UFUNCTION(BlueprintImplementableEvent)
	void			OnDeath();
protected:
	UFUNCTION(BlueprintCallable)
	virtual void	ClearAllMember();
	
/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent*		GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	URaidBossAbilitySystemComponent*		GetRaidBossAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable)
	const URaidBossCharacterStatusAttributeSet*	GetCharacterStatusAttributeSet() const;
	UFUNCTION(BlueprintCallable)
	double									GetExperience() const;
	UFUNCTION(BlueprintCallable)
	double									GetMaxExperience() const;
	UFUNCTION(BlueprintCallable)
	int										GetSkillPoint() const;
	UFUNCTION(BlueprintCallable)
	bool									IsCharacterStateTurnOn(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable)
	float									GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float									GetAttackPower() const;
	
	UFUNCTION(BlueprintCallable)
	void			TurnOnCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable)
	void			TurnOffCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable)
	void			IncreaseSkillPoint();
	UFUNCTION(BlueprintCallable)
	void			DecreaseSkillPoint();
	UFUNCTION(BlueprintCallable)
	virtual void	GiveExperience(double Exp);
	virtual void	CharacterLevelUp(float IncrementNum);

/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Ability")
	TObjectPtr<URaidBossAbilitySystemComponent>		AbilitySystemComponent;
	UPROPERTY()
	const URaidBossCharacterStatusAttributeSet*		CharacterStatusAttributeSet;

	int32	CharacterStateBitMask;
	double	Experience = 0;// 골드 경험치 어트리뷰트로 ㄱㄱ
	double	MaxExperience = 100;
	int		SkillPoint = 5;
	float	CharacterLevel = 1;
};
