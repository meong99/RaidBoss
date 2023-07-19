#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RaidBossChangeCharacterStatus.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossChangeCharacterStatus : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URaidBossChangeCharacterStatus();
/*
*	----------- Overrided
*/
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
	void	SetProperty(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float Magnitude) const;
	void	CheckAndApplyHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyDefencePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
/*
*	----------- Access
*/

	float	GetBonusHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusDefensePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseDefensePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
/*
*	----------- Member Variables
*/
};
