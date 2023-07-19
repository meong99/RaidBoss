#include "Character/Enemy/RaidBossEnemyBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ARaidBossEnemyBase::ARaidBossEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
}

void ARaidBossEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Enermy");
}

void ARaidBossEnemyBase::CreateSkillObjects()
{
	ARaidBossEnemyControllerBase*	EnemyController;
	
	EnemyController = GetRiadBossEnemyController();
	if (EnemyController && AbilitySystemComponent)
	{
		const auto PlayerSkillClasses = EnemyController->GetSkillClasses();
		for (auto SkillClass : PlayerSkillClasses)
		{
			FGameplayAbilitySpecHandle SpecHandle;
			URaidBossSkillBase*	SkillCDO = Cast<URaidBossSkillBase>(SkillClass->GetDefaultObject());
			AbilitySystemComponent->GiveAbilityWithoutDuplication(SkillClass, SpecHandle, static_cast<int32>(SkillCDO->AbilityInputID));

			URaidBossSkillBase* SkillObject = Cast<URaidBossSkillBase>(AbilitySystemComponent->GetAbilityByClass(SkillClass));
			if (SkillObject)
				SkillObjects.Add(SkillObject);
		}
	}
}

ARaidBossEnemyControllerBase* ARaidBossEnemyBase::GetRiadBossEnemyController() const
{
	return Cast<ARaidBossEnemyControllerBase>(GetController());
}
