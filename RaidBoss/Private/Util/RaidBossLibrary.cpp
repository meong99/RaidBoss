#include "Util/RaidBossLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

bool RaidBossLibrary::SphereTraceSingleForObjects(const UWorld* World, const FVector Start, const FVector End, float CapsuleRadius,
	FHitResult& ResultOut, const EDrawDebugTrace::Type DrawDebugType, const TArray<AActor*>& IgnoreActors,
	const TArray<ECollisionChannel>& CollisionChannel)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	bool bTraceComplex = false;
	bool bIgnoreSelf = true;
	
	for (auto ObjectType : CollisionChannel)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ObjectType));
	}
	
	return (UKismetSystemLibrary::SphereTraceSingleForObjects(World, Start, End, CapsuleRadius,
		ObjectTypes, bTraceComplex, IgnoreActors, DrawDebugType, ResultOut, bIgnoreSelf));
}
