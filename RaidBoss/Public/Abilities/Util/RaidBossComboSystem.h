#pragma once

#include "CoreMinimal.h"
#include "Util/RaidBossTickableGameObject.h"
#include "RaidBossComboSystem.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossComboSystem : public UObject, public FRaidBossTickableGameObject
{
	GENERATED_BODY()
public:
	URaidBossComboSystem();
/*
*	----------- Overrided
*/
	virtual void	Tick(float DeltaTime) override;
/*
*	----------- Binded by Delegate
*/
public:
	void	SetResetTime(float Time);
/*
*	----------- Other Method
*/
/*
*	----------- Access
*/
public:
	uint8	GetCurrentStack() const;
	void	SetMaxStack(uint8 Value);
	void	IncreaseStack();
	void	ResetStack();
/*
*	----------- Member Variables
*/
protected:
	uint8			Stack = 0;
	uint8			MaxStack = 1;
	float			ResetTime = 1;
	float			ElapsedTime = 0;
};
