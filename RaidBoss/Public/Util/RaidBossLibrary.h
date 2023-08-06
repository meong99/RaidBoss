#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

namespace RaidBossLibrary
{
	bool	SphereTraceSingleForObjects(const UWorld* World, const FVector Start, const FVector End, float CapsuleRadius, FHitResult& ResultOut,
										const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None,
										const TArray<AActor*>& IgnoreActors = TArray<AActor*>{},
										const TArray<ECollisionChannel>& CollisionChannel = TArray<ECollisionChannel>{ECollisionChannel::ECC_WorldStatic});
}
