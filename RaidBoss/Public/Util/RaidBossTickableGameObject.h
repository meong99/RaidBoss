#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

class RAIDBOSS_API FRaidBossTickableGameObject : public FTickableGameObject
{
public:
	FRaidBossTickableGameObject();
	~FRaidBossTickableGameObject();

	void			SetIsTickable(bool Tickable);
	virtual bool	IsTickable() const override;
	virtual TStatId	GetStatId() const override;
	
protected:
	bool	bIsTickable = false;
};
