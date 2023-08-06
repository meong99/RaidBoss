#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RaidBossDamageCalculation.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	URaidBossDamageCalculation();
	
	virtual void	Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
											FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
