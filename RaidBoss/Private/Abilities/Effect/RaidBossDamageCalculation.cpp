#include "Abilities/Effect/RaidBossDamageCalculation.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AdditionalAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AdditionalDefencePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AdditionalAttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AdditionalDefencePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, Health, Target, false);
	}
};

static FDamageStatics& DamageStatic()
{
	static FDamageStatics Static;

	return Static;
}

URaidBossDamageCalculation::URaidBossDamageCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatic().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().AdditionalAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().DefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().AdditionalDefencePowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().HealthDef);
}

void URaidBossDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	AActor* Test = ExecutionParams.GetTargetAbilitySystemComponent()->GetOwnerActor();

	float SourceTotalAttackPower = 0;
	float SourceAdditionalAttackPower = 0;
	float TargetTotalDefensePower = 0;
	float TargetAdditionalDefencePower = 0;
	float TargetHealth = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AttackPowerDef, EvaluateParameters, SourceTotalAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AdditionalAttackPowerDef, EvaluateParameters, SourceAdditionalAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefensePowerDef, EvaluateParameters, TargetTotalDefensePower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AdditionalDefencePowerDef, EvaluateParameters, TargetAdditionalDefencePower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().HealthDef, EvaluateParameters, TargetHealth);

	TargetTotalDefensePower += TargetAdditionalDefencePower;
	SourceTotalAttackPower += SourceAdditionalAttackPower;
	
	if (TargetHealth > 0)
	{
		float DefensiveConstant = 100;
		float DefenseRate = TargetTotalDefensePower / (TargetTotalDefensePower + DefensiveConstant);
		float TotalDamage = ceil(SourceTotalAttackPower * (1 - DefenseRate));
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatic().HealthProperty, EGameplayModOp::Additive, -TotalDamage));
	}
}
