// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/MonsterBase.h"
#include "Character/RaidBossAnimBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "Character/Monster/MonsterSpawner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HealthBarWidget.h"

AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	bUseControllerRotationYaw = false;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	HealthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBar->SetVisibility(false);
	
	Tags.Add("Enemy");
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetGenericTeamId({MONSTER_TEAM_ID});
	
	OwnerSpawner = Cast<AMonsterSpawner>(Owner);
}

void AMonsterBase::Destroyed()
{
	if (HealthBar && HealthBar->GetWidget())
	{
		HealthBar->GetWidget()->RemoveFromParent();
	}
	Super::Destroyed();
}

void AMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MonsterController = Cast<ARaidBossEnemyControllerBase>(NewController);
	
	if (UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(HealthBar->GetWidget()))
	{
		HealthBarWidget->SetWidgetOwner(this);
	}
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

FVector AMonsterBase::GetSpawnerLocation() const
{
	FVector SpawnerLocation = {};

	if (OwnerSpawner.IsValid())
	{
		SpawnerLocation = OwnerSpawner->GetActorLocation();
	}
	
	return SpawnerLocation;
}

float AMonsterBase::GetDistanceBetweenSpawner() const
{
	float	DistanceBetweenSpawner = 0;

	if (OwnerSpawner.IsValid())
	{
		DistanceBetweenSpawner = (OwnerSpawner->GetActorLocation() - GetActorLocation()).Length();
	}
	
	return DistanceBetweenSpawner;
}

ARaidBossEnemyControllerBase* AMonsterBase::GetRiadBossEnemyController() const
{
	return MonsterController.Get();
}

void AMonsterBase::SetMonsterHealthBarVisibility(bool NewVisible) const
{
	if (HealthBar)
	{
		if (MonsterType == EMonsterType::BossMonster)
		{
			if (HealthBar->GetWidget() && NewVisible)
			{
				HealthBar->GetWidget()->AddToViewport();
			}
			else if (HealthBar->GetWidget() && !NewVisible)
			{
				HealthBar->GetWidget()->RemoveFromParent();
			}
		}
		else if (MonsterType == EMonsterType::NormalMonster)
		{
			HealthBar->SetVisibility(NewVisible);
		}
	}
}
