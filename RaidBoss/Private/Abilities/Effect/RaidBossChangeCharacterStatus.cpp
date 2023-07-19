#include "Abilities/Effect/RaidBossChangeCharacterStatus.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

struct FStatusStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Mana);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxMana);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

	FStatusStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, Mana, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, MaxMana, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AttackPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, DefensePower, Target, false);
	}
};

static FStatusStatics& StatusStatic()
{
	static FStatusStatics Static;

	return Static;
}

URaidBossChangeCharacterStatus::URaidBossChangeCharacterStatus()
{
	RelevantAttributesToCapture.Add(StatusStatic().HealthDef);
	RelevantAttributesToCapture.Add(StatusStatic().MaxHealthDef);
	RelevantAttributesToCapture.Add(StatusStatic().ManaDef);
	RelevantAttributesToCapture.Add(StatusStatic().MaxManaDef);
	RelevantAttributesToCapture.Add(StatusStatic().AttackPowerDef);
	RelevantAttributesToCapture.Add(StatusStatic().DefensePowerDef);
}

void URaidBossChangeCharacterStatus::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	CheckAndApplyMaxHealth(ExecutionParams, OutExecutionOutput);
	CheckAndApplyHealth(ExecutionParams, OutExecutionOutput);
	CheckAndApplyMaxMana(ExecutionParams, OutExecutionOutput);
	CheckAndApplyMana(ExecutionParams, OutExecutionOutput);
	CheckAndApplyAttackPower(ExecutionParams, OutExecutionOutput);
	CheckAndApplyDefencePower(ExecutionParams, OutExecutionOutput);
}

void URaidBossChangeCharacterStatus::SetProperty(FGameplayEffectCustomExecutionOutput& OutExecutionOutput,
	const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp,float Magnitude) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(InAttribute, ModifierOp, Magnitude));
}

void URaidBossChangeCharacterStatus::CheckAndApplyHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BaseValue = GetBaseHealth(ExecutionParams);
	float BonusValue = GetBonusHealth(ExecutionParams);
	float MaxHealth = GetBaseMaxHealth(ExecutionParams);

	if (BaseValue + BonusValue < 0)
	{
		BonusValue -= (BaseValue - BonusValue);
	}
	else if (BaseValue + BonusValue > MaxHealth)
	{
		BonusValue -= (BaseValue + BonusValue - MaxHealth);
	}
	
	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().HealthProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusMaxHealth(ExecutionParams);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().MaxHealthProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusMana(ExecutionParams);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().ManaProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusMaxMana(ExecutionParams);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().MaxManaProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusAttackPower(ExecutionParams);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().AttackPowerProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyDefencePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusDefensePower(ExecutionParams);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().DefensePowerProperty, EGameplayModOp::Additive, BonusValue);
	}
}

float URaidBossChangeCharacterStatus::GetBonusHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().HealthDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().HealthDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().MaxHealthDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().MaxHealthDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().ManaDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().ManaDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().MaxManaDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().MaxManaDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().AttackPowerDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().AttackPowerDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusDefensePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().DefensePowerDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseDefensePower(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().DefensePowerDef, OutValue);
	
	return OutValue;
}
