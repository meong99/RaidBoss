#include "Character/Enemy/RaidBossEnemyBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ARaidBossEnemyBase::ARaidBossEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;

	bUseControllerRotationYaw = false;
	
	Tags.Add("Enemy");
}

void ARaidBossEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	ApplyCharacterStatusEffect();
}

ARaidBossEnemyControllerBase* ARaidBossEnemyBase::GetRiadBossEnemyController() const
{
	return Cast<ARaidBossEnemyControllerBase>(GetController());
}
