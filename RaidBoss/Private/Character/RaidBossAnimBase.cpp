#include "Character/RaidBossAnimBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URaidBossAnimBase::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);
	
	ACharacter* Character = Cast<ACharacter>(GetOwningActor());
	
	if (IsValid(Character) == false)
		return;

	FRotator Temp = Character->GetBaseAimRotation() - Character->GetActorRotation();
	
	Temp.Normalize();
	Roll = Temp.Roll;
	Pitch = Temp.Pitch;
	Yaw = Temp.Yaw;

	Speed = Character->GetVelocity().Size2D();
	bIsInAir = Character->GetCharacterMovement()->IsFalling();
	bIsAccelerating = (Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f);

	FVector Velocity = Character->GetVelocity();
	
	Velocity.Normalize();
	
	FVector RotatedVector = Character->GetActorRotation().UnrotateVector(Velocity);
	
	MoveForward = RotatedVector.X;
	MoveRight = RotatedVector.Y;

	if (IsAccelerating() == true)
	{
		SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
	else
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
}

ARaidBossCharacterBase* URaidBossAnimBase::GetOwningCharacter() const
{
	return Cast<ARaidBossCharacterBase>(GetOwningActor());
}
