#include "Character/RaidBossAnimBase.h"

#include "AbilitySystemComponent.h"
#include "Character/RaidBossCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Management/RaidBossGameplayTags.h"

void URaidBossAnimBase::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);
	
	ARaidBossCharacterBase* Character = GetOwningCharacter();
	
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
	InputMoveForward = FMath::FInterpTo(InputMoveForward, Character->GetInputMoveForward(), deltaTime, InterpSpeed);
	InputMoveRight = FMath::FInterpTo(InputMoveRight, Character->GetInputMoveRight(), deltaTime, InterpSpeed);;

	bIsDuringAlign = Character->IsDuringAlign();
	bIsTurnLeft = Character->IsTurnLeft();
		
	if (IsAccelerating() == true)
	{
		SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
	else
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}

	UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();

	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(RaidBossGameplayTags::Get().Attack))
	{
		bIsAttacking = true;
	}
	else
	{
		bIsAttacking = false;
	}
}

ARaidBossCharacterBase* URaidBossAnimBase::GetOwningCharacter() const
{
	return Cast<ARaidBossCharacterBase>(GetOwningActor());
}
