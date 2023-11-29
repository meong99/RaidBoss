// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/MonsterBase.h"
#include "Character/RaidBossAnimBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	bUseControllerRotationYaw = false;
	
	Tags.Add("Enemy");
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterBase::PossessedBy(AController* NewController)
{
	if (MonsterKey != INDEX_NONE)
	{
		FMonsterInfo* MonsterInfo = GetMonsterInfo();
	
		if (MonsterInfo)
		{
			InitMonster(*MonsterInfo);
			ApplyCharacterDefaultSpecEffectToSelf();
			EquipWeapon(MonsterInfo->MonsterWeaponKey);
		}
	}
	
	Super::PossessedBy(NewController);
}

void AMonsterBase::InitMonster(const FMonsterInfo& MonsterInfoRow)
{
	GetMesh()->SetSkeletalMesh(MonsterInfoRow.SkeletalMesh);
	GetMesh()->SetAnimClass(MonsterInfoRow.AnimBP);
	MonsterType = MonsterInfoRow.MonsterType;
	CharacterDefaultSpec = MonsterInfoRow.MonsterSpec;
	MonsterReward = MonsterInfoRow.MonsterReward;
	CurrentPlayerState = EPlayerState::Alive;

	InitAnimationData(MonsterInfoRow.MonsterAnimationTable);
}

FMonsterInfo* AMonsterBase::GetMonsterInfo() const
{
	const TCHAR*	MonsterDataTablePath = MONSTER_DATATABLE_PATH;
	
	UDataTable* MonsterDataTable = LoadObject<UDataTable>(nullptr, MonsterDataTablePath);

	if (MonsterDataTable)
	{
		TArray<FMonsterInfo*> MonsterInfos;
		FMonsterInfo* MonsterData = nullptr;

		MonsterDataTable->GetAllRows<FMonsterInfo>(FString(), MonsterInfos);

		if (MonsterInfos.IsValidIndex(MonsterKey))
		{
			MonsterData = MonsterInfos[MonsterKey];
		}
		
		return MonsterData;
	}

	return nullptr;
}

ARaidBossEnemyControllerBase* AMonsterBase::GetRiadBossEnemyController() const
{
	return Cast<ARaidBossEnemyControllerBase>(GetController());
}
