// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

#define PLAYER_TAG "Player"

UENUM(BlueprintType)
enum class ERaidBossAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Attack
};