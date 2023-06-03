#include "Enermy/RaidBossEnermyBase.h"
#include "Enermy/RaidBossEnermyAnimBase.h"
#include "Enermy/RaidBossEnermyControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ARaidBossEnermyBase::ARaidBossEnermyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
}

void ARaidBossEnermyBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Enermy");
	AnimInstance = Cast<URaidBossEnermyAnimBase>(GetMesh()->GetAnimInstance());
}

float ARaidBossEnermyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float	damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return damage;
}

void ARaidBossEnermyBase::PlayDeathMontage()
{
	if (IsValid(AnimInstance) == false)
		return;

	AnimInstance->PlayDeathMontage();
}

void ARaidBossEnermyBase::PlayAttackMontage()
{
	if (IsValid(AnimInstance) == false)
		return;

	AnimInstance->PlayAttackMontage();
}
