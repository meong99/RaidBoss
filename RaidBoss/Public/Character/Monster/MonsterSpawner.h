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
    UFUNCTION(BlueprintCallable, Category="Raid Boss | Monster Spawner")
    void    RequestToSpawnMonster();

    UFUNCTION(BlueprintCallable, Category="Raid Boss | Monster Spawner")
    void    RequestToStopSpawning();

    void    ResetSpawner();

    int32   GetCurrentSpawnedMonsterNumber() const;

    /*
     *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */
    UFUNCTION(BlueprintCallable)
    int32   GetDeadMonsterNumber() const { return DeadMonsterNumber; }

protected:
    bool    SpawnMonster();

    void    DestroyDeadMonsters();

protected:
    /*
     *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY(EditDefaultsOnly, Category="Raid Boss | Monster Spawner")
    TObjectPtr<UDataTable>  MonsterDataTable;

    UPROPERTY(EditAnywhere, Category="Raid Boss | Monster Spawner")
    int32                   MaximumSpawnMonster = 2;

    UPROPERTY(EditAnywhere, Category="Raid Boss | Monster Spawner")
    int32                   MonsterKey = INDEX_NONE;

    //몬스터 스폰 주기
    UPROPERTY(EditAnywhere, Category="Raid Boss | Monster Spawner")
    float                   SpawnTimeCycle = 30;

    /*
     *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
     */

    //
    UPROPERTY()
    TArray<AMonsterBase*>   SpawnedMonsters;

    int32   DeadMonsterNumber = 0;

    float   Timer;
};
