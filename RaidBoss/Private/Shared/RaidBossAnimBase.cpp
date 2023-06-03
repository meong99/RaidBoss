#include "Shared/RaidBossAnimBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URaidBossAnimBase::URaidBossAnimBase()
{
}

void URaidBossAnimBase::NativeUpdateAnimation(float deltaTime)
{
	APawn* pawnOwner = TryGetPawnOwner();

	if (IsValid(pawnOwner) == false)
		return;

	ACharacter* character = Cast<ACharacter>(pawnOwner);

	if (IsValid(character) == false)
		return;

	FRotator rTemp = pawnOwner->GetBaseAimRotation() - pawnOwner->GetActorRotation();

	rTemp.Normalize();
	Roll = rTemp.Roll;
	Pitch = rTemp.Pitch;
	Yaw = rTemp.Yaw;

	FRotator actorRotation = pawnOwner->GetActorRotation() - RotationLastTick;

	actorRotation.Normalize();
	YawDelta = FMath::FInterpTo(YawDelta, (actorRotation.Yaw / deltaTime / 7.0f), deltaTime, 6.0f);
	RotationLastTick = pawnOwner->GetActorRotation();

	Speed = pawnOwner->GetVelocity().Size();
	bIsInAir = pawnOwner->GetMovementComponent()->IsFalling();
	bIsAccelorating = (character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f);
}

UAnimMontage* URaidBossAnimBase::GetAttackMontage() const
{
	return AttackMonatage;
}
