#pragma once

#include "AbilityType.generated.h"

UENUM(BlueprintType)
enum class ECharacterAbilityInputs : uint8
{
	None	UMETA(DisplayName = "None"),
	Confirm	UMETA(DisplayName = "Confirm"),
	Cancel	UMETA(DisplayName = "Cancel"),
	Move	UMETA(DisplayName = "Move"),
	Jump	UMETA(DisplayName = "Jump"),
	Look	UMETA(DisplayName = "Look"),
};
