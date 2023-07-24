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
	void	SaveAggregatorFromExecutedSpec(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	void	CheckAndApplyHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyDefencePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	SetProperty(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float Magnitude) const;

/*
*	----------- Access
*/
	float	GetBonusHealthFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float	GetBonusMaxHealthFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float	GetBonusManaFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float	GetBonusMaxManaFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float	GetBonusAttackPowerFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float	GetBonusDefensePowerFromExecutedSpec(const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	
	float	GetBaseHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBaseDefensePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusHealthAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxHealthAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusManaAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxManaAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusAttackPowerAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusDefensePowerAllEcecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
/*
*	----------- Member Variables
*/

private:
	static TMap<FGameplayEffectAttributeCaptureDefinition, float>	ExecutedValueFromSpec;
};
