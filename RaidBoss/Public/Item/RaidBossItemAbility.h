#pragma once

#include "Shared/RaidBossSharedAbility.h"
#include "RaidBossItemAbility.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API URaidBossItemAbility : public URaidBossSharedAbility
{
	GENERATED_BODY()

public:
	URaidBossItemAbility();
};

