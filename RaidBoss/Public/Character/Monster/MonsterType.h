// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "character/CharacterTpye.h"
#include "Engine/DataTable.h"
#include "Equipment/EquipmentType.h"
#include "MonsterType.generated.h"

class AMonsterBase;
class ARaidBossEnemyControllerBase;
class URaidBossAnimBase;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	None UMETA(DisplayName = "None"),
	NormalMonster UMETA(DisplayName = "NormalMonster"),
	BossMonster UMETA(DisplayName = "BossMonster"),
};

USTRUCT(BlueprintType)
struct FMonsterAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Monster | Animation")
	TObjectPtr<UAnimMontage>	SpawnMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Monster | Animation")
	TObjectPtr<UAnimMontage>	AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Monster | Animation")
	TObjectPtr<UAnimMontage>	DeathMontage;
};

USTRUCT(BlueprintType)
struct FMonsterReward
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Monster | Reward")
	float	Gold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Monster | Reward")
	TArray<uint8>	ItemKeys;
};

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMonsterBase>		MonsterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh>		SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URaidBossAnimBase>	AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable>			MonsterAnimationTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMonsterType					MonsterType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterSpec					MonsterSpec;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponKey						MonsterWeaponKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMonsterReward					MonsterReward;
};
