// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterType.h"
#include "Character/RaidBossCharacterBase.h"
#include "MonsterBase.generated.h"

UCLASS()
class RAIDBOSS_API AMonsterBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	
public:
	void	InitMonster(const FMonsterInfo& MonsterInfoRow);
	void	ApplyMonsterEffectSpec();

protected:
	
public:
	ARaidBossEnemyControllerBase* GetRiadBossEnemyController() const;
	EPlayerState	GetMonsterState() const { return CurrentPlayerState; }
	EMonsterType	GetMonsterType() const { return MonsterType; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	EMonsterType					MonsterType;
	UPROPERTY(BlueprintReadOnly)
	FMonsterAnimations				MonsterAnimations;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss")
	FCharacterSpec					MonsterSpec;
	UPROPERTY(BlueprintReadWrite)
	FMonsterReward					MonsterReward;
};
