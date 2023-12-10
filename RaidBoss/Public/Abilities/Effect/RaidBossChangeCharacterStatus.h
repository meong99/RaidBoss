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

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	void	SaveAggregatorFromExecutedSpec(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	void	CheckAndApplyHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	CheckAndApplyDefencePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void	SetProperty(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float Magnitude) const;

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
	float	GetBonusHealthAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxHealthAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusManaAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusMaxManaAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusAttackPowerAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;
	float	GetBonusDefensePowerAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;

private:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */

	// 변경 필요 - 스태틱이라 다른 클래스도 공용으로 사용하다보니 중첩되는 것 같음
	static TMap<FGameplayEffectAttributeCaptureDefinition, float>	ExecutedValueFromSpec;
};
