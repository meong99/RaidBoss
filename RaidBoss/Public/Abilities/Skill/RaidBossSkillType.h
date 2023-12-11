#pragma once

#include "Engine/DataTable.h"
#include "Global/RaidBoss.h"
#include "RaidBossSkillType.generated.h"

UENUM(BlueprintType)
enum class ERaidBossSkillType : uint8
{
    None UMETA(DisplayName = "None"),
    Attack UMETA(DisplayName = "Attack"),
    Buff UMETA(DisplayName = "Buff"),
    Debuff UMETA(DisplayName = "Debuff"),
    Util UMETA(DisplayName = "Util"),
};

USTRUCT(BlueprintType)
struct FRaidBossSkillInfo
{
    GENERATED_BODY()

public:
    FRaidBossSkillInfo()
    {
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ERaidBossSkillType SkillType = ERaidBossSkillType::None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UTexture2D> SkillTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString SkillName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int SkillCost = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int RequirePoint = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame)
    int SkillLevel = 0;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int MaximumSkillLevel = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int MinimumSkillLevel = 0;
};

enum ESkillRequestType
{
    None,
    IncreaseSkillLevel,
    DecreaseSkillLevel
};
