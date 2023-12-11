#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RaidBossGameInstance.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API URaidBossGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    URaidBossGameInstance();

    virtual void Init() override;
};
