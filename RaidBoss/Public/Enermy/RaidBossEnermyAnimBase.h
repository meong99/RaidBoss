#pragma once

#include "CoreMinimal.h"
#include "Shared/RaidBossAnimBase.h"
#include "RaidBossEnermyAnimBase.generated.h"

UCLASS(Abstract)
class RAIDBOSS_API URaidBossEnermyAnimBase : public URaidBossAnimBase
{
	GENERATED_BODY()

public:
	virtual void	PlayDeathMontage();
	virtual void	PlayAttackMontage();

protected:

};
