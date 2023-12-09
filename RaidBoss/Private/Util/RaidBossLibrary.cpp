// Fill out your copyright notice in the Description page of Project Settings.

#include "Util/RaidBossLibrary.h"
#include "EngineUtils.h"
#include "Character/RaidBossCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<AActor*> URaidBossLibrary::GetActorsInAngle(const UObject* WorldContextObject, const FVector& Location,
                                                     const FVector& Direction, float Radius,
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
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, Location + FRotator(0, Angle / 2, 0).RotateVector(Direction) * Radius,
			TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, Location + FRotator(0, -Angle / 2, 0).RotateVector(Direction) * Radius,
			TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
	}

	for (auto ActorClass : FilterClasses)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			if (bIgnoreDistanceZ && FVector::DistXY(Location, Actor->GetActorLocation()) > Radius)
			{
				continue;
			}
			
			if (!bIgnoreDistanceZ && FVector::Dist(Location, Actor->GetActorLocation()) > Radius)
			{
				continue;
			}
			
			FVector DirectionToTarget = (Actor->GetActorLocation() - Location).GetSafeNormal2D();
			float	tmpangle = FMath::RadiansToDegrees(acos(DirectionToTarget.Dot(Direction)));
			
			bool	bIsInAngle = tmpangle <= Angle / 2;
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

TArray<AActor*> URaidBossLibrary::GetActorsInRectangle(const UObject* WorldContextObject, const FVector& Location,
	const FVector& Direction, float ForwardDistance, float SideDistance, const TArray<TSubclassOf<AActor>>& FilterClasses,
	const TArray<TSubclassOf<AActor>>& IgnoreClasses, bool bIgnoreDistanceZ, bool DrawDebug)
{
	TArray<AActor*>	Result;
	
	if (WorldContextObject == nullptr || Direction.IsZero())
	{
		return Result;
	}

	if (DrawDebug)
	{
		FVector FwdEnd =  Location + Direction * ForwardDistance;
		FVector LeftStart = FwdEnd + (FRotator(0, -90, 0).RotateVector(Direction) * (SideDistance / 2));
		FVector RightEnd =	FwdEnd + (FRotator(0, 90, 0).RotateVector(Direction) * (SideDistance / 2));
		FVector	StartRight = Location + RightEnd - FwdEnd;
		FVector	RightToLeft = LeftStart - RightEnd;
		
		TArray<AActor*> TMP;
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, FwdEnd, TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, LeftStart, TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, RightEnd, TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, RightEnd, StartRight, TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
		UKismetSystemLibrary::LineTraceSingle(WorldContextObject, StartRight, StartRight + RightToLeft, TraceTypeQuery1, false, TMP, EDrawDebugTrace::ForDuration, HitResult, true);
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	
	for (auto ActorClass : FilterClasses)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			
			if (IgnoreClasses.Contains(Actor->GetClass()))
			{
				continue;
			}

			FVector LocationToTarget = Actor->GetActorLocation() - Location;

			if (Direction.Dot(LocationToTarget.GetSafeNormal2D()) < 0)
			{
				continue;
			}
			
			float	TargetDistance = LocationToTarget.Length();
			float	RadianBetweenDirAndTarget = acos(LocationToTarget.GetSafeNormal2D().Dot(Direction));

			if (cos(RadianBetweenDirAndTarget) * TargetDistance <= ForwardDistance &&
				sin(RadianBetweenDirAndTarget) * TargetDistance <= SideDistance / 2)
			{
				Result.Add(Actor);
			}
		}
	}
	
	return Result;
}

FVector URaidBossLibrary::GetFloorLocation(const UObject* WorldContextObject, const FVector& StartLocation, float TraceLength, bool bDrawDebugLine)
{
	FVector DownVector = FVector{0, 0, -1} * TraceLength;
	EDrawDebugTrace::Type DebugTrace = bDrawDebugLine ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	FHitResult HitResult;
	
	bool bWasHit = UKismetSystemLibrary::LineTraceSingleForObjects(WorldContextObject, StartLocation, StartLocation + DownVector,
		{ UEngineTypes::ConvertToObjectType(ECC_WorldStatic) }, false, {}, DebugTrace, HitResult, true);

	if (bWasHit)
	{
		return HitResult.Location;
	}

	return StartLocation;
}

bool URaidBossLibrary::LineTraceForObjectFromCamera(const UObject* WorldContextObject,
	const ARaidBossCharacterBase* CharacterBase, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	const TArray<AActor*>& ActorsToIgnore, FHitResult& OutHit, FVector& OutTraceEndLocation, bool bIgnoreSelf, float TraceLength, bool bDrawDebugLine)
{
	if (CharacterBase == nullptr)
	{
		return false;
	}

	FVector CameraLocation = CharacterBase->GetActorLocation() + CharacterBase->GetFollowCamera()->GetRelativeLocation();
	FVector	CameraForward = CharacterBase->GetControlRotation().Vector();
	float	FinalTracingLength = (CharacterBase->GetActorLocation() - CameraLocation).Length() + TraceLength;
	
	EDrawDebugTrace::Type DrawDebugType = bDrawDebugLine ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	OutTraceEndLocation = CameraLocation + CameraForward * FinalTracingLength;
	
	return UKismetSystemLibrary::LineTraceSingleForObjects(WorldContextObject, CameraLocation,
		OutTraceEndLocation, ObjectTypes, false, ActorsToIgnore, DrawDebugType, OutHit,
		bIgnoreSelf);
}

bool URaidBossLibrary::GetVelocityToLaunchActor(const UObject* WorldContextObject, FVector& OutLaunchVelocity,
                                                FVector StartPos, FVector EndPos, float ArcParam, float OverrideGravityZ)
{
	return UGameplayStatics::SuggestProjectileVelocity_CustomArc(WorldContextObject, OutLaunchVelocity, StartPos, EndPos, OverrideGravityZ, ArcParam);
}

UParticleSystemComponent* URaidBossLibrary::SpawnEmitterAtLocation(UWorld* World, UParticleSystem* EmitterTemplate,
                                                                   const FTransform& SpawnTransform, bool bAutoDestroy, EPSCPoolMethod PoolingMethod, bool bAutoActivateSystem)
{
	return UGameplayStatics::SpawnEmitterAtLocation(World, EmitterTemplate, SpawnTransform, bAutoDestroy, PoolingMethod, bAutoActivateSystem);
}

FVector URaidBossLibrary::RotateVector(FVector Vector, FRotator Rotator)
{
	return Rotator.RotateVector(Vector);
}
