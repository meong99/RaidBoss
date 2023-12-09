// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/MonsterSpawner.h"
#include "GameFramework/GameStateBase.h"
#include "RaidBossGameStateBase.generated.h"

USTRUCT(BlueprintType)
struct FStageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32					StageNumber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32					MaximumSpawnableMonsterNumber;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float					SpawnTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable>	MonsterDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int32>			MonsterKeysToSpawn;
};

// 제거 예정

UCLASS()
class RAIDBOSS_API ARaidBossGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARaidBossGameStateBase();
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	bool	RequestStartingNextStage();
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void	InitializeGameState();
	void	FindDeployedMonsterSpawner();
	void	LoadStageDataTable();
	bool	SpawnMonsterAtRandomSpawner();
	bool	CanSpawnMonster() const;
	bool	CheckSpawnTime(float DeltaSeconds) const;
	bool	IsMonsterSpawningFinished() const;
	bool	AreEveryMonstersDead() const;
	void	CheckDeadMonsters();
	UFUNCTION(BlueprintNativeEvent)
	void	GameClear();
	void	GameClear_Implementation();
	void	StageClear();
	void	ResetState();
	void	ResetGame();

	void	SetCurrentStageData();
	void	SetSpawnableMonsterInfos(const FStageData* StageData);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Stage")
	int32						TotalStageNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable>		StageDataTable = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TArray<AMonsterSpawner*>	Spawners;
	UPROPERTY(BlueprintReadOnly)
	int32						ClearedStageNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	int32						SpawnedMonsterNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	int32						DeadMonsterNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	int32						CurrentStageNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FMonsterInfo>		SpawnableMonsterInfos;
	UPROPERTY(BlueprintReadOnly)
	int32						MaximumSpawnableMonsterNumber = 0;
	UPROPERTY(BlueprintReadOnly)
	float						SpawnTimer = 1.f;
};
