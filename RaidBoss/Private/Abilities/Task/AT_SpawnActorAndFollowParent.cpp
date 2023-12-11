// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Util/RaidBossLibrary.h"

UAT_SpawnActorAndFollowParent::UAT_SpawnActorAndFollowParent()
{
}

UAT_SpawnActorAndFollowParent* UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(UGameplayAbility* OwningAbility,
    TSubclassOf<AActor> InSpawnClass, const ACharacter* InParentActor,
    float InToMoveForward/* = 0*/, FVector InScale/* = {1, 1, 1}*/,
    bool bInAttachLocation/* = true*/, bool bInFollowRotation/* = true*/)
{
    UAT_SpawnActorAndFollowParent* MyObj = NewAbilityTask<UAT_SpawnActorAndFollowParent>(OwningAbility);

    MyObj->SpawnClass = InSpawnClass;
    MyObj->ParentActor = InParentActor;
    MyObj->ToMoveForward = InToMoveForward;
    MyObj->Scale = InScale;
    MyObj->bAttachLocation = bInAttachLocation;
    MyObj->bFollowRotation = bInFollowRotation;

    return MyObj;
}

void UAT_SpawnActorAndFollowParent::ExternalCancel()
{
    bTickingTask = false;

    Super::ExternalCancel();
}

void UAT_SpawnActorAndFollowParent::TickTask(float DeltaTime)
{
    if (SpawnedActor && ParentActor.IsValid())
    {
        if (bAttachLocation)
        {
            FVector Location =
                URaidBossLibrary::GetFloorLocation(
                    ParentActor.Get(), ParentActor->GetActorLocation()) +
                    UKismetMathLibrary::GetForwardVector(FRotator{0, ParentActor->GetControlRotation().Yaw, 0}) *
                    ToMoveForward;

            SpawnedActor->SetActorLocation(Location);
        }
        if (bFollowRotation)
        {
            FRotator Rotation = OriginRotator + FRotator{0, ParentActor->GetControlRotation().Yaw, 0};

            SpawnedActor->SetActorRotation(Rotation);
        }
        else
        {
            FRotator Rotation = OriginRotator + UKismetMathLibrary::MakeRotFromX(
                UKismetMathLibrary::GetForwardVector(FRotator{0, ParentActor->GetControlRotation().Yaw, 0}));

            SpawnedActor->SetActorRotation(Rotation);
        }
    }
}

void UAT_SpawnActorAndFollowParent::Activate()
{
    UWorld* CurrentWorld = GEngine->GetWorldFromContextObject(ParentActor.Get(), EGetWorldErrorMode::LogAndReturnNull);

    if (CurrentWorld && ParentActor.Get())
    {
        SpawnedActor = CurrentWorld->SpawnActorDeferred<AActor>(SpawnClass, FTransform::Identity);

        if (SpawnedActor && ParentActor.Get())
        {
            FVector Location = URaidBossLibrary::GetFloorLocation(ParentActor.Get(), ParentActor->GetActorLocation()) +
                UKismetMathLibrary::GetForwardVector(FRotator{0, ParentActor->GetControlRotation().Yaw, 0}) *
                ToMoveForward;

            OriginRotator = SpawnedActor->GetActorRotation();

            SpawnedActor->FinishSpawning({FRotator::ZeroRotator, Location, Scale});
            
            bTickingTask = true;
        }
    }

    Super::Activate();
}

void UAT_SpawnActorAndFollowParent::OnDestroy(bool bInOwnerFinished)
{
    bTickingTask = false;

    if (SpawnedActor)
    {
        SpawnedActor->Destroy();
    }
    SpawnedActor = nullptr;

    Super::OnDestroy(bInOwnerFinished);
}
