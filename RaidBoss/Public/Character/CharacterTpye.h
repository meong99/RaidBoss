﻿#pragma once

#include "CharacterTpye.generated.h"

class URaidBossItemBase;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    None UMETA(DisplayName = "None"),
    Alive UMETA(DisplayName = "Alive"),
    Dead UMETA(DisplayName = "Dead")
};

USTRUCT(BlueprintType)
struct FCharacterAnimations
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> DeathMontage;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> TookDamage;
};

USTRUCT(BlueprintType)
struct FCharacterSpec
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float MaxHealth;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float MaxMana;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float AttackPower;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float AttackRange;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float DefencePower;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float MoveSpeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Information")
    float AttackSpeed;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character | Item")
    URaidBossItemBase* Item;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character | Item")
    int32 Amount;
};
