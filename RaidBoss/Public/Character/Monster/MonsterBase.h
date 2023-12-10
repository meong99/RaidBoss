// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterType.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "MonsterBase.generated.h"

#define MONSTER_DATATABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_Monsters.DT_Monsters'")

class AMonsterSpawner;

UCLASS()
class RAIDBOSS_API AMonsterBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

protected:
	virtual void	PossessedBy(AController* NewController) override;

	virtual void	BeginPlay() override;
	
	virtual void	Destroyed() override;

	
public:
	void	InitMonster(const FMonsterInfo& MonsterInfoRow);

	FVector	GetSpawnerLocation() const;

	float	GetDistanceBetweenSpawner() const;

	void	SetMonsterHealthBarVisibility(bool	NewVisible) const;

/*
 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Monster")
	ARaidBossEnemyControllerBase*	GetRiadBossEnemyController() const { return MonsterController.Get(); }
	ECharacterState					GetMonsterState() const { return CurrentCharacterState; }
	EMonsterType					GetMonsterType() const { return MonsterType; }
	
	// 플레이어 추격 종료 조건 달성했을 때부터 이 시간이 지나면 추격 종료
	float							GetTimerForStopChase() const { return TimerForStopChase; }
	float							GetMaxChasingDistance() const { return MaxChasingDistance; }
	
protected:
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	TWeakObjectPtr<ARaidBossEnemyControllerBase>	MonsterController;
	
	TWeakObjectPtr<AMonsterSpawner>	OwnerSpawner;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Monster")
	TObjectPtr<UWidgetComponent>	HealthBar;

	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Monster")
	EMonsterType					MonsterType;
	
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Monster")
	FMonsterAnimations				MonsterAnimations;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss | Monster")
	FMonsterReward					MonsterReward;

	// 플레이어 추격 종료 조건 달성했을 때부터 이 시간이 지나면 추격 종료
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Monster")
	float	TimerForStopChase = 10;

	// 스포너에서 떨어질 수 있는 최대 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Monster")
	float	MaxChasingDistance = 1500;
	
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
};
