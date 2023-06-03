#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RaidBoss/RaidBoss.h"
#include "RaidBossSharedAbility.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API URaidBossSharedAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URaidBossSharedAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	ERaidBossAbilityInputID	AbilityInputID = ERaidBossAbilityInputID::None;
};
