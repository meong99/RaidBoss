// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataPassingObject.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class RAIDBOSS_API UDataPassingObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, float>	FloatValue;
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FVector>	VectorValue;
};
