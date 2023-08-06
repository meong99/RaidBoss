#include "Abilities/Effect/RaidBossChangeCharacterStatus.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

TMap<FGameplayEffectAttributeCaptureDefinition, float> URaidBossChangeCharacterStatus::ExecutedValueFromSpec;

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
	ExecutedValueFromSpec.Reset();
	SaveAggregatorFromExecutedSpec(ExecutionParams);
	
	CheckAndApplyMaxHealth(ExecutionParams, OutExecutionOutput);
	CheckAndApplyHealth(ExecutionParams, OutExecutionOutput);
	CheckAndApplyMaxMana(ExecutionParams, OutExecutionOutput);
	CheckAndApplyMana(ExecutionParams, OutExecutionOutput);
	CheckAndApplyAttackPower(ExecutionParams, OutExecutionOutput);
	CheckAndApplyDefencePower(ExecutionParams, OutExecutionOutput);
}

void URaidBossChangeCharacterStatus::SaveAggregatorFromExecutedSpec(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	const FGameplayEffectSpec&					OwningSpec = ExecutionParams.GetOwningSpec();
	const UGameplayEffect*						EffectDef = OwningSpec.Def;
	TArray<FGameplayEffectExecutionDefinition>	ExecutionFromDef;
	
	if (EffectDef)
		ExecutionFromDef = EffectDef->Executions;
	
	for (auto CurExecDef : ExecutionFromDef)
	{
		for (auto CurScopedMod : CurExecDef.CalculationModifiers)
		{
			float	ExcutedValue;
			bool	bIscalculationSuccessed = CurScopedMod.ModifierMagnitude.AttemptCalculateMagnitude(ExecutionParams.GetOwningSpec(), ExcutedValue);

			if (bIscalculationSuccessed)
			{
				ExecutedValueFromSpec.Add(CurScopedMod.CapturedAttribute, ExcutedValue);
			}
		}
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BaseValue = GetBaseHealth(ExecutionParams);
	float BonusValue = GetBonusHealthFromExecutedSpec(StatusStatic().HealthDef);
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
	float BonusValue = GetBonusMaxManaFromExecutedSpec(StatusStatic().MaxManaDef);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().MaxHealthProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BaseValue = GetBaseMana(ExecutionParams);
	float BonusValue = GetBonusManaFromExecutedSpec(StatusStatic().ManaDef);
	float MaxMana = GetBaseMaxMana(ExecutionParams);
	
	if (BaseValue + BonusValue < 0)
	{
		BonusValue -= (BaseValue - BonusValue);
	}
	else if (BaseValue + BonusValue > MaxMana)
	{
		BonusValue -= (BaseValue + BonusValue - MaxMana);
	}
	
	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().ManaProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusMaxManaFromExecutedSpec(StatusStatic().MaxManaDef);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().MaxManaProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusAttackPowerFromExecutedSpec(StatusStatic().AttackPowerDef);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().AttackPowerProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::CheckAndApplyDefencePower(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float BonusValue = GetBonusDefensePowerFromExecutedSpec(StatusStatic().DefensePowerDef);

	bool WillChange = (BonusValue != 0);
	
	if (WillChange)
	{
		SetProperty(OutExecutionOutput, StatusStatic().DefensePowerProperty, EGameplayModOp::Additive, BonusValue);
	}
}

void URaidBossChangeCharacterStatus::SetProperty(FGameplayEffectCustomExecutionOutput& OutExecutionOutput,
	const FGameplayAttribute& InAttribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp,float Magnitude) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(InAttribute, ModifierOp, Magnitude));
}

float URaidBossChangeCharacterStatus::GetBonusHealthFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxHealthFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBonusManaFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxManaFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBonusAttackPowerFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBonusDefensePowerFromExecutedSpec(
	const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float	BonusValue = 0;
	float*	FindedValue = ExecutedValueFromSpec.Find(InCaptureDef);
	
	if (FindedValue)
	{
		BonusValue = *FindedValue;
	}
	
	return BonusValue;
}

float URaidBossChangeCharacterStatus::GetBaseHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().HealthDef, OutValue);

	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMaxHealth(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().MaxHealthDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().ManaDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseMaxMana(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().MaxManaDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseAttackPower(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().AttackPowerDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBaseDefensePower(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(StatusStatic().DefensePowerDef, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusHealthAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().HealthDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxHealthAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	float OutValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().MaxHealthDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusManaAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().ManaDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusMaxManaAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().MaxManaDef, EvaluateParameters, OutValue);
	
	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusAttackPowerAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	FAggregatorEvaluateParameters	EvaluateParameters;
	float							OutValue;
	
	OutValue = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().AttackPowerDef, EvaluateParameters, OutValue);

	return OutValue;
}

float URaidBossChangeCharacterStatus::GetBonusDefensePowerAllExecuted(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	float OutValue = 0;

	FAggregatorEvaluateParameters EvaluateParameters;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(StatusStatic().DefensePowerDef, EvaluateParameters, OutValue);
	
	return OutValue;
}
