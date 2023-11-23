﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Task/AT_DashCharacter.h"

#include "Character/Player/RaidBossPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UAT_DashCharacter* UAT_DashCharacter::CreateDashCharacterTask(UGameplayAbility* OwningAbility, FVector InDirection, float InDashSpeed,
                                                              float InDashDistance)
{
	UAT_DashCharacter* MyObj = NewAbilityTask<UAT_DashCharacter>(OwningAbility);

	// MyObj->bIgnoreGravity = bInIgnoreGravity;
	MyObj->DashDistance = InDashDistance;
	MyObj->DashSpeed = InDashSpeed;
	
	if (InDirection.IsZero())
	{
		InDirection = {1, 0, 0};
	}
	
	MyObj->Direction = InDirection.GetSafeNormal();
	
	return MyObj;
}

void UAT_DashCharacter::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	ARaidBossPlayerBase*	CharacterBase = Cast<ARaidBossPlayerBase>(GetOwnerActor());
	
	if (CharacterBase)
	{
		// FVector	CharacterLocation = CharacterBase->GetActorLocation();
		// FVector NewDestination = FVector{Destination.X, Destination.Y, CharacterLocation.Z};
		//
		// NewDestination = FMath::VInterpConstantTo(CharacterLocation, NewDestination, DeltaTime, DashSpeed);

		// PastDistance += (NewDestination - CharacterLocation).Length();
		
		// CharacterBase->SetActorLocation(NewDestination);
		CharacterBase->AddMovementInput(Direction);
	}

	if (CharacterBase && (CharacterBase->GetActorLocation() - OriginLocation).Length() >= DashDistance)
	{
		EndTask();
	}
}

void UAT_DashCharacter::ExternalCancel()
{
	Super::ExternalCancel();
}

void UAT_DashCharacter::Activate()
{
	Super::Activate();
	ARaidBossPlayerBase*	CharacterBase = Cast<ARaidBossPlayerBase>(GetOwnerActor());

	if (CharacterBase)
	{
		bTickingTask = true;
		
		// FVector	CharacterLocation = CharacterBase->GetActorLocation();
		
		// Destination = CharacterLocation + Direction * DashDistance;
		OriginLocation = CharacterBase->GetActorLocation();
		OriginMovementSpeed = CharacterBase->GetCharacterMovement()->MaxWalkSpeed;

		CharacterBase->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		CharacterBase->GetCharacterMovement()->MaxAcceleration *= 2;
		CharacterBase->SetActorRotation(FRotator{0, CharacterBase->GetControlRotation().Yaw, 0 });
	}
}

void UAT_DashCharacter::OnDestroy(bool bInOwnerFinished)
{
	bTickingTask = false;
	
	ARaidBossPlayerBase*	CharacterBase = Cast<ARaidBossPlayerBase>(GetOwnerActor());
	
	if (CharacterBase)
	{
		CharacterBase->GetCharacterMovement()->MaxWalkSpeed = OriginMovementSpeed;
		CharacterBase->GetCharacterMovement()->MaxAcceleration /= 2;
	}
	
	Super::OnDestroy(bInOwnerFinished);
}
