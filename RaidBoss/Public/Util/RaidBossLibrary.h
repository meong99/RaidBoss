// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RaidBossLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API URaidBossLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss Library", meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext, AutoCreateRefTerm="IgnoreClasses"))
	static TArray<AActor*>	GetActorsInAngle2D(const UObject* WorldContextObject, const FVector& Location, const FVector& Direction, float Distance, 
		const TArray<TSubclassOf<AActor>>& FilterClasses,
		const TArray<TSubclassOf<AActor>>& IgnoreClasses,
		float Angle = 45, bool bIgnoreDistanceZ = true, bool DrawDebug = false);
};
