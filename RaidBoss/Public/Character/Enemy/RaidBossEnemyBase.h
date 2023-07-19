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

/*
 *	----------- Overrided
 */
protected:
	virtual void	BeginPlay() override;

/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
	void	CreateSkillObjects();
/*
*	----------- Access
*/
	ARaidBossEnemyControllerBase*	GetRiadBossEnemyController() const;
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TArray<URaidBossSkillBase*>	SkillObjects;
};
