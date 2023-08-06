#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERaidBossAbilityInputID : uint8
{
	None  UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	DefaultAttack UMETA(DisplayName = "DefaultAttack"),
	Q UMETA(DisplayName = "Q"),
	E UMETA(DisplayName = "E"),
	R UMETA(DisplayName = "R"),
	Dash UMETA(DisplayName = "Dash"),
	Death UMETA(DisplayName = "Death"),
	Respawn UMETA(DisplayName = "Respawn"),
};
