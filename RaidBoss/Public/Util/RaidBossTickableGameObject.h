#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

class RAIDBOSS_API FRaidBossTickableGameObject : public FTickableGameObject
{
public:
	FRaidBossTickableGameObject();
	
	virtual ~FRaidBossTickableGameObject() override;
	
	virtual bool	IsTickable() const override;
	
	virtual TStatId	GetStatId() const override;

	void			SetIsTickable(bool Tickable);
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	bool	bIsTickable = false;
};
