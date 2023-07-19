#include "Character/RaidBossAnimBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/RaidBossCharacterBase.h"

URaidBossAnimBase::URaidBossAnimBase()
{
}

void URaidBossAnimBase::NativeUpdateAnimation(float deltaTime)
{
	AActor*		OwingActor;
	ACharacter* Character;
	FVector		Velocity;
	FVector		RotatedVector;
	FRotator	Temp;

	OwingActor = GetOwningActor();
	if (IsValid(OwingActor) == false)
		return;
	Character = Cast<ACharacter>(OwingActor);
	if (IsValid(Character) == false)
		return;

	Temp = Character->GetBaseAimRotation() - Character->GetActorRotation();
	Temp.Normalize();
	Roll = Temp.Roll;
	Pitch = Temp.Pitch;
	Yaw = Temp.Yaw;

	Speed = Character->GetVelocity().Size2D();
	bIsInAir = Character->GetCharacterMovement()->IsFalling();
	bIsAccelorating = (Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f);

	Velocity = Character->GetVelocity();
	Velocity.Normalize();
	RotatedVector = Character->GetActorRotation().UnrotateVector(Velocity);
	MoveForward = RotatedVector.X;
	MoveRight = RotatedVector.Y;

	if (IsAccelorating() == true)
		SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	else
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
}

void URaidBossAnimBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

ARaidBossCharacterBase* URaidBossAnimBase::GetOwningCharacter() const
{
	return Cast<ARaidBossCharacterBase>(GetOwningActor());
}
