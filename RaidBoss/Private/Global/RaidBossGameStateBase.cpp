// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/RaidBossGameStateBase.h"
#include "Kismet/GameplayStatics.h"

ARaidBossGameStateBase::ARaidBossGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARaidBossGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckDeadMonsters();
	
	if (CanSpawnMonster() && CheckSpawnTime(DeltaSeconds))
	{
		if (SpawnMonsterAtRandomSpawner())
		{
			SpawnedMonsterNumber++;
		}
	}
	else if (IsMonsterSpawningFinished() && AreEveryMonstersDead())
	{
		StageClear();

		if (TotalStageNumber == ClearedStageNumber)
		{
			GameClear_Implementation();
		}
	}
}

bool ARaidBossGameStateBase::RequestStartingNextStage()
{
	if (TotalStageNumber > ClearedStageNumber &&
		CurrentStageNumber == ClearedStageNumber)
	{
		ResetState();
		CurrentStageNumber++;
		
		SetCurrentStageData();
		SetActorTickEnabled(true);

		for (auto Spawner : Spawners)
		{
			Spawner->ResetSpawner();
		}
		return true;
	}

	return false;
}

void ARaidBossGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	InitializeGameState();
}

void ARaidBossGameStateBase::InitializeGameState()
{
	
	if (StageDataTable == nullptr || TotalStageNumber == ClearedStageNumber)
	{
		ResetGame();
		FindDeployedMonsterSpawner();
		LoadStageDataTable();
	}
}

void ARaidBossGameStateBase::FindDeployedMonsterSpawner()
{
	TArray<AActor*>	SpawnersInWorld;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawner::StaticClass(), SpawnersInWorld);

	for (const auto& SpawnerInWorld : SpawnersInWorld)
	{
		AMonsterSpawner* SpawnerToAdd = Cast<AMonsterSpawner>(SpawnerInWorld);

		if (SpawnerToAdd)
		{
			Spawners.Add(SpawnerToAdd);
		}
	}
}

void ARaidBossGameStateBase::LoadStageDataTable()
{
	const TCHAR*	StageDataTableLink = TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_StageDatas.DT_StageDatas'");
	
	StageDataTable = LoadObject<UDataTable>(nullptr, StageDataTableLink);
	if (StageDataTable)
	{
		const int32 TotalDataNum = StageDataTable->GetRowNames().Num();
		
		if (TotalDataNum < TotalStageNumber)
		{
			TotalStageNumber = TotalDataNum;
		}
	}
}

bool ARaidBossGameStateBase::SpawnMonsterAtRandomSpawner()
{
	if (Spawners.IsEmpty() || SpawnableMonsterInfos.IsEmpty())
	{
		return false;
	}
	static int PickedSpawner = 0;
	static int PickedMonster = 0;

	if (SpawnableMonsterInfos.Num() == PickedMonster)
	{
		PickedMonster = 0;
	}
	if (Spawners.Num() == PickedSpawner)
	{
		PickedSpawner = 0;
	}
	
	AMonsterSpawner*	SpawnerToSpawn = Spawners[PickedSpawner];
	bool				bSpawnSuccess = false;
	
	if (SpawnerToSpawn)
	{
		bSpawnSuccess = SpawnerToSpawn->RequestSpawningMonster(SpawnableMonsterInfos[PickedMonster]);

		if (bSpawnSuccess)
		{
			PickedSpawner++;
			PickedMonster++;
		}
	}

	return bSpawnSuccess;
}

bool ARaidBossGameStateBase::CanSpawnMonster() const
{
	if (MaximumSpawnableMonsterNumber > SpawnedMonsterNumber)
	{
		return true;
	}
	
	return false;
}

bool ARaidBossGameStateBase::CheckSpawnTime(float DeltaSeconds) const
{
	static float ElapsedTime = SpawnTimer;

	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= SpawnTimer)
	{
		ElapsedTime -= SpawnTimer;

		return true;
	}
	
	return false;
}

bool ARaidBossGameStateBase::IsMonsterSpawningFinished() const
{
	if (MaximumSpawnableMonsterNumber <= SpawnedMonsterNumber)
	{
		return true;
	}
	
	return false;
}

bool ARaidBossGameStateBase::AreEveryMonstersDead() const
{
	if (SpawnedMonsterNumber > DeadMonsterNumber)
	{
		return false;
	}

	return true;
}

void ARaidBossGameStateBase::CheckDeadMonsters()
{
	int32	DeadMonsters = 0;

	for (const auto& Spawner : Spawners)
	{
		DeadMonsters += Spawner->GetDeadMonsterNumber();
	}

	DeadMonsterNumber = DeadMonsters;
	
}

void ARaidBossGameStateBase::GameClear_Implementation()
{
	GameClear();
}

void ARaidBossGameStateBase::StageClear()
{
	ClearedStageNumber++;
	
	SetActorTickEnabled(false);
}

void ARaidBossGameStateBase::ResetState()
{
	SpawnedMonsterNumber = 0;
	DeadMonsterNumber = 0;
	SpawnableMonsterInfos.Reset();
	MaximumSpawnableMonsterNumber = 0;
	SpawnTimer = 1.f;
}

void ARaidBossGameStateBase::ResetGame()
{
	Spawners.Reset();
	ClearedStageNumber = 0;
	CurrentStageNumber = 0;
}

void ARaidBossGameStateBase::SetCurrentStageData()
{
	if (StageDataTable == nullptr)
	{
		return;
	}
	
	const FStageData* StageData = StageDataTable->FindRow<FStageData>(*FString::FromInt(CurrentStageNumber), FString());

	if (StageData)
	{
		SpawnTimer = StageData->SpawnTimer;
		MaximumSpawnableMonsterNumber = StageData->MaximumSpawnableMonsterNumber;

		SetSpawnableMonsterInfos(StageData);
	}
}

void ARaidBossGameStateBase::SetSpawnableMonsterInfos(const FStageData* StageData)
{
	if (StageData->MonsterDataTable == nullptr)
	{
		return;
	}

	for (int i = 0; i < StageData->MonsterKeysToSpawn.Num(); i++)
	{
		const FMonsterInfo*	MonsterInfo = StageData->MonsterDataTable->FindRow<FMonsterInfo>(
										  *FString::FromInt(StageData->MonsterKeysToSpawn[i]), FString());
		
		if (MonsterInfo)
		{
			SpawnableMonsterInfos.Add(*MonsterInfo);
		}
	}
}
