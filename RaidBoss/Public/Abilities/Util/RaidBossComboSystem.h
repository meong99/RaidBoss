#pragma once

#include "CoreMinimal.h"
#include "Util/RaidBossTickableGameObject.h"
#include "RaidBossComboSystem.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossComboSystem : public UObject, public FRaidBossTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void	Tick(float DeltaTime) override;

	uint8	GetCurrentStack() const;
	
	void	IncreaseStack();
	void	ResetStack();
	void	SetMaxStack(uint8 Value);
	void	SetResetTime(float Time);

private:
	uint8	Stack = 0;
	uint8	MaxStack = 1;
	float	ResetTime = 1;
	float	ElapsedTime = 0;
};
