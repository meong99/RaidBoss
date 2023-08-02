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

	UFUNCTION(BlueprintImplementableEvent, Category = "Raid Boss | Character Base")
	void			OnDeath();
protected:
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	virtual void	ClearAllMember();
	
	void	ApplyCharacterStatusEffect();

/*
 *	----------- Access
 */
public:
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	virtual UAbilitySystemComponent*		GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	URaidBossAbilitySystemComponent*		GetRaidBossAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	const URaidBossCharacterStatusAttributeSet*	GetCharacterStatusAttributeSet() const;
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	double	GetExperience() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	double	GetMaxExperience() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	bool	IsCharacterStateTurnOn(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	float	GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	float	GetAttackPower() const;
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	void			TurnOnCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	void			TurnOffCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	virtual void	GiveExperience(double Exp);
	virtual void	CharacterLevelUp(float IncrementNum);

/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Character Base")
	TObjectPtr<URaidBossAbilitySystemComponent>		AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TSubclassOf<UGameplayEffect>					CharacterStatusEffect;
	UPROPERTY()
	const URaidBossCharacterStatusAttributeSet*		CharacterStatusAttributeSet;
	int32	CharacterStateBitMask;
	double	Experience = 0;// 골드 경험치 어트리뷰트로 ㄱㄱ
	double	MaxExperience = 100;
	float	CharacterLevel = 1;
};
