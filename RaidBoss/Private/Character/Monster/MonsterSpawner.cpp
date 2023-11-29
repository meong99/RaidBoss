// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/MonsterSpawner.h"
#include "Character/Monster/MonsterBase.h"

AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

bool AMonsterSpawner::RequestSpawningMonster(FMonsterInfo MonsterInfoToSpawn)
{
	UWorld* CurrentWorld = GetWorld();
	
	if (CurrentWorld && MonsterInfoToSpawn.MonsterType != EMonsterType::None)
	{
		AMonsterBase*	SpawnedMonster = CurrentWorld->SpawnActorDeferred<AMonsterBase>(MonsterInfoToSpawn.MonsterClass, FTransform::Identity, this);

		if (SpawnedMonster == nullptr)
		{
			return false;
		}

		SpawnedMonster->InitMonster(MonsterInfoToSpawn);
		SpawnedMonsters.Add(SpawnedMonster);
		SpawnedMonster->FinishSpawning(GetTransform());
		SpawnedMonster->SpawnDefaultController();

		if (MonsterInfoToSpawn.MonsterWeaponKey.WeaponType != EWeaponType::None)
		{
			SpawnedMonster->EquipWeapon(MonsterInfoToSpawn.MonsterWeaponKey);
		}

		SetActorTickEnabled(true);
		
		return true;
	}
	return false;
}

int32 AMonsterSpawner::GetCurrentSpawnedMonsterNumber() const
{
	return SpawnedMonsters.Num();
}

int32 AMonsterSpawner::GetDeadMonsterNumber() const
{
	return DeadMonsterNumber;
}

void AMonsterSpawner::ResetSpawner()
{
	DeadMonsterNumber = 0;
	SpawnedMonsters.Reset();
}

void AMonsterSpawner::DestroyDeadMonsters()
{
	for (int i = 0; i < SpawnedMonsters.Num(); i++)
	{
		if (SpawnedMonsters[i]->GetMonsterState() == EPlayerState::Dead)
		{
			AMonsterBase* Tmp = SpawnedMonsters[i];
			
			DeadMonsterNumber++;
			SpawnedMonsters.RemoveAt(i);
			Tmp->Destroy();
		}
	}
}

void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyDeadMonsters();

	if (SpawnedMonsters.IsEmpty())
	{
		SetActorTickEnabled(false);
	}
}

