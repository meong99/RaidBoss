// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Task/AT_SpawnActorAndFollowCamera.h"
#include "Character/RaidBossCharacterBase.h"
#include "Util/RaidBossLibrary.h"

UAT_SpawnActorAndFollowCamera::UAT_SpawnActorAndFollowCamera()
{
}

UAT_SpawnActorAndFollowCamera* UAT_SpawnActorAndFollowCamera::SpawnActorAndFollowCamera(UGameplayAbility* OwningAbility,
                                                                                        TSubclassOf<AActor> InSpawnClass, const ARaidBossCharacterBase* InParentActor,
                                                                                        float Range, FVector InScale/* = {1, 1, 1}*/,
                                                                                        bool bInDoTrackingLocation/* = true*/, bool bInFollowRotation/* = false*/)
{
	UAT_SpawnActorAndFollowCamera* MyObj = NewAbilityTask<UAT_SpawnActorAndFollowCamera>(OwningAbility);

	MyObj->SpawnClass = InSpawnClass;
	MyObj->ParentActor = InParentActor;
	MyObj->Scale = InScale;
	MyObj->Range = Range;
	MyObj->DoTrackingLocation = bInDoTrackingLocation;
	MyObj->bFollowRotation = bInFollowRotation;

	return MyObj;
}

void UAT_SpawnActorAndFollowCamera::ExternalCancel()
{
	bTickingTask = false;
	
	Super::ExternalCancel();
}

void UAT_SpawnActorAndFollowCamera::TickTask(float DeltaTime)
{
	if (SpawnedActor && ParentActor.IsValid())
	{
		FVector		Location = GetSpawnLocation();
		FRotator	Rotation = OriginRotator + FRotator{0, ParentActor->GetControlRotation().Yaw, 0};

		if (DoTrackingLocation)
		{
			SpawnedActor->SetActorLocation(Location);
		}
		if (bFollowRotation)
		{
			SpawnedActor->SetActorRotation(Rotation);
		}
	}
}

void UAT_SpawnActorAndFollowCamera::Activate()
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	if (CurrentWorld && ParentActor.Get())
	{
		SpawnedActor = CurrentWorld->SpawnActor<AActor>(SpawnClass, {FRotator::ZeroRotator, GetSpawnLocation(), Scale});

		if (SpawnedActor && ParentActor.Get())
		{
			OriginRotator = SpawnedActor->GetActorRotation();
	
			SpawnedActor->SetActorRotation(OriginRotator);
			
			bTickingTask = true;
		}
	}
	
	Super::Activate();
}

void UAT_SpawnActorAndFollowCamera::OnDestroy(bool bInOwnerFinished)
{
	bTickingTask = false;

	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
	}
	SpawnedActor = nullptr;
	
	Super::OnDestroy(bInOwnerFinished);
}

FVector UAT_SpawnActorAndFollowCamera::GetSpawnLocation()
{
	FHitResult	HitResult;
	FVector		TraceEndLocation;
	
	bool bTracingComplete = URaidBossLibrary::LineTraceForObjectFromCamera(this, ParentActor.Get(), {UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
		{}, HitResult, TraceEndLocation, true, Range);

	FVector SpawnLocation;
	
	if (bTracingComplete)
	{
		SpawnLocation = URaidBossLibrary::GetFloorLocation(this, HitResult.Location);
	}
	else
	{
		SpawnLocation = URaidBossLibrary::GetFloorLocation(this, TraceEndLocation);
	}

	return SpawnLocation;
}
