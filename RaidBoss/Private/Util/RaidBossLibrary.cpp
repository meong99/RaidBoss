// Fill out your copyright notice in the Description page of Project Settings.

#include "Util/RaidBossLibrary.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<AActor*> URaidBossLibrary::GetActorsInAngle2D(const UObject* WorldContextObject, const FVector& Location,
                                                     const FVector& Direction, float Distance,
                                                     const TArray<TSubclassOf<AActor>>& FilterClasses,
                                                     const TArray<TSubclassOf<AActor>>& IgnoreClasses,
                                                     float Angle/* = 45*/, bool bIgnoreDistanceZ/* = true*/, bool DrawDebug/* = false*/)
{
	TArray<AActor*>	Result;
	
	if (WorldContextObject == nullptr || Direction.IsZero())
	{
		return Result;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (DrawDebug)
	{
		TArray<AActor*> TMP;
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, Location + FRotator(0, Angle, 0).RotateVector(Direction) * Distance,
			TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, Location + FRotator(0, -Angle, 0).RotateVector(Direction) * Distance,
			TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
	}

	for (auto ActorClass : FilterClasses)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			if (bIgnoreDistanceZ && FVector::DistXY(Location, Actor->GetActorLocation()) > Distance)
			{
				continue;
			}
			
			if (!bIgnoreDistanceZ && FVector::Dist(Location, Actor->GetActorLocation()) > Distance)
			{
				continue;
			}
			
			FVector DirectionToTarget = (Actor->GetActorLocation() - Location).GetSafeNormal2D();
			bool	bIsInAngle = DirectionToTarget.Dot(Direction) > cos(Angle);
			if (DirectionToTarget.IsZero() == false && bIsInAngle == false)
			{
				continue;
			}
			if (IgnoreClasses.Contains(Actor->GetClass()) == false)
			{
				Result.Add(Actor);
			}
		}
	}
	
	return Result;
}
