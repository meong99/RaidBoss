// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterType.h"
#include "Character/RaidBossCharacterBase.h"
#include "MonsterBase.generated.h"

#define MONSTER_DATATABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_Monsters.DT_Monsters'")

UCLASS()
class RAIDBOSS_API AMonsterBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
public:
	void	InitMonster(const FMonsterInfo& MonsterInfoRow);

protected:
	FMonsterInfo*	GetMonsterInfo() const;
public:
	ARaidBossEnemyControllerBase* GetRiadBossEnemyController() const;
	EPlayerState	GetMonsterState() const { return CurrentPlayerState; }
	EMonsterType	GetMonsterType() const { return MonsterType; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Raid Boss")
	int32	MonsterKey = INDEX_NONE;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss")
	EMonsterType					MonsterType;
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss")
	FMonsterAnimations				MonsterAnimations;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss")
	FCharacterSpec					MonsterSpec;
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss")
	FMonsterReward					MonsterReward;
};
