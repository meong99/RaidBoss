#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

namespace RaidBossLibrary
{
	// A and B remove the decimal point if the margin is 0.
	bool	NearbyCompareFloat(float A, float B, float Margin = 0);
	bool	SphereTraceSingleForObjects(const UWorld* World, const FVector Start, const FVector End, float CapsuleRadius, FHitResult& ResultOut,
										const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration,
										const TArray<AActor*>& IgnoreActors = TArray<AActor*>{},
										const TArray<ECollisionChannel>& CollisionChannel = TArray<ECollisionChannel>{ECollisionChannel::ECC_WorldStatic});
}
