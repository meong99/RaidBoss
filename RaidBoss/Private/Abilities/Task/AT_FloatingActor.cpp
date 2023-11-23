// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Task/AT_FloatingActor.h"

#include "Character/RaidBossCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_FloatingActor* UAT_FloatingActor::CreateFloatingActorTask(UGameplayAbility* OwningAbility, float InHeight,
                                                              float InFloatingSpeed, float InFloatingTime, float InFallingSpeed)
{
	UAT_FloatingActor* MyObj = NewAbilityTask<UAT_FloatingActor>(OwningAbility);

	MyObj->Height = InHeight;
	MyObj->FloatingTime = InFloatingTime;
	MyObj->FloatingSpeed = InFloatingSpeed;
	MyObj->FallingSpeed = InFallingSpeed;
	
	return MyObj;
}

void UAT_FloatingActor::ExternalCancel()
{
	Super::ExternalCancel();
}

void UAT_FloatingActor::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (OwnerCharacter && ActivateTime < FloatingTime)
	{
		InterpolateDestinationXY();
		
		FVector	NextDestination = FMath::VInterpConstantTo(OwnerCharacter->GetActorLocation(), Destination,
			DeltaTime, FloatingSpeed);
		
		OwnerCharacter->SetActorLocation(NextDestination);

		ActivateTime += DeltaTime;
	}
	else if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MovementMode = MOVE_Walking;
		OwnerCharacter->GetCharacterMovement()->GravityScale = FallingSpeed;
	}
}

void UAT_FloatingActor::Activate()
{
	Super::Activate();

	OwnerCharacter = Cast<ARaidBossCharacterBase>(GetOwnerActor());

	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MovementMode = MOVE_Flying;
		
		Destination = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorUpVector() * Height;
		bTickingTask = true;
	}
}

void UAT_FloatingActor::OnDestroy(bool bInOwnerFinished)
{
	bTickingTask = false;

	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MovementMode = MOVE_Walking;
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1;
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAT_FloatingActor::InterpolateDestinationXY()
{
	if (OwnerCharacter)
	{
		Destination.X = OwnerCharacter->GetActorLocation().X;
		Destination.Y = OwnerCharacter->GetActorLocation().Y;
	}
}
