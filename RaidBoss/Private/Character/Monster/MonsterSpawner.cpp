// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/MonsterSpawner.h"
#include "Character/Monster/MonsterBase.h"

AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyDeadMonsters();

	if (GetCurrentSpawnedMonsterNumber() < MaximumSpawnMonster)
	{
		Timer += DeltaTime;
	}
	
	if (Timer >= SpawnTimeCycle &&
		GetCurrentSpawnedMonsterNumber() < MaximumSpawnMonster)
	{
		Timer = 0;
		
		SpawnMonster();
	}
}

void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Timer = SpawnTimeCycle;

	SetActorTickEnabled(false);
}

void AMonsterSpawner::RequestToSpawnMonster()
{
	SetActorTickEnabled(true);
}

void AMonsterSpawner::RequestToStopSpawning()
{
	SetActorTickEnabled(false);
}

void AMonsterSpawner::ResetSpawner()
{
	DeadMonsterNumber = 0;
	SpawnedMonsters.Reset();
}

int32 AMonsterSpawner::GetCurrentSpawnedMonsterNumber() const
{
	return SpawnedMonsters.Num();
}

bool AMonsterSpawner::SpawnMonster()
{
	UWorld* CurrentWorld = GetWorld();

	if (MonsterDataTable == nullptr && MonsterKey == INDEX_NONE)
	{
		return false;
	}
	
	const FMonsterInfo*	MonsterInfo = MonsterDataTable->FindRow<FMonsterInfo>(
										  *FString::FromInt(MonsterKey), FString());
	
	if (CurrentWorld && MonsterInfo && MonsterInfo->MonsterType != EMonsterType::None)
	{
		AMonsterBase*	SpawnedMonster = CurrentWorld->SpawnActorDeferred<AMonsterBase>(MonsterInfo->MonsterClass, FTransform::Identity, this);

		if (SpawnedMonster == nullptr)
		{
			return false;
		}

		SpawnedMonster->InitMonster(*MonsterInfo);
		SpawnedMonsters.Add(SpawnedMonster);

		SpawnedMonster->FinishSpawning(GetTransform());
		SpawnedMonster->SpawnDefaultController();
		
		if (MonsterInfo->MonsterWeaponKey.WeaponType != EWeaponType::None)
		{
			SpawnedMonster->EquipWeapon(MonsterInfo->MonsterWeaponKey);
		}

		SetActorTickEnabled(true);
		
		return true;
	}
	
	return false;
}

void AMonsterSpawner::DestroyDeadMonsters()
{
	for (int i = 0; i < SpawnedMonsters.Num(); i++)
	{
		if (SpawnedMonsters[i]->GetMonsterState() == ECharacterState::Dead)
		{
			AMonsterBase* Tmp = SpawnedMonsters[i];
			
			DeadMonsterNumber++;
			SpawnedMonsters.RemoveAt(i);
			Tmp->Destroy();
		}
	}
}
