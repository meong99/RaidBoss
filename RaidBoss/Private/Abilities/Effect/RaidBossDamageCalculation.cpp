#include "Abilities/Effect/RaidBossDamageCalculation.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AdditialnalAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, AdditialnalAttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URaidBossCharacterStatusAttributeSet, DefensePower, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatic().AdditialnalAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().DefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatic().HealthDef);
}

void URaidBossDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceAbilityComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilityComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor*	 SourceActor							= SourceAbilityComponent ? SourceAbilityComponent->GetAvatarActor() : nullptr;
	AActor*	 TargetActor							= TargetAbilityComponent ? SourceAbilityComponent->GetAvatarActor() : nullptr;
	const FGameplayEffectSpec& Spec					= ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags			= Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags			= Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float SourceAttackPower = 0;
	float SourceAdditialnalAttackPower = 0;
	float TargetDefensePower = 0;
	float TargetHealth = 0;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AdditialnalAttackPowerDef, EvaluateParameters, SourceAdditialnalAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().HealthDef, EvaluateParameters, TargetHealth);

	if (TargetHealth > 0)
	{
		float DefensiveConstant = 100;
		float DefenseRate = TargetDefensePower / (TargetDefensePower + DefensiveConstant);
		float TotalDamage = ceil((SourceAttackPower + SourceAdditialnalAttackPower) * (1 - DefenseRate));
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatic().HealthProperty, EGameplayModOp::Additive, -TotalDamage));
	}
}
