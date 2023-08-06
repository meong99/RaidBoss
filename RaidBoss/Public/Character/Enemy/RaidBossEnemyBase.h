#pragma once

#include "CoreMinimal.h"
#include "Character/RaidBossCharacterBase.h"
#include "RaidBossEnemyBase.generated.h"

class ARaidBossEnemyControllerBase;
class URaidBossEnemyAnimBase;

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossEnemyBase : public ARaidBossCharacterBase
{
	GENERATED_BODY()
public:
	ARaidBossEnemyBase();

	virtual void	BeginPlay() override;

	ARaidBossEnemyControllerBase*	GetRiadBossEnemyController() const;
};
