// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterType.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

class AMonsterBase;

UCLASS()
class RAIDBOSS_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()

public:
	AMonsterSpawner();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool	RequestSpawningMonster(FMonsterInfo MonsterInfoToSpawn);
	UFUNCTION(BlueprintCallable)
	int32	GetCurrentSpawnedMonsterNumber() const;
	int32	GetDeadMonsterNumber() const;
	void	ResetSpawner();
	
protected:
	void	DestroyDeadMonsters();

protected:
	TArray<AMonsterBase*>	SpawnedMonsters;
	int32					DeadMonsterNumber = 0;
};
