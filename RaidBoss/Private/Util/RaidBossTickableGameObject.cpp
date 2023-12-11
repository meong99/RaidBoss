#include "Util/RaidBossTickableGameObject.h"

FRaidBossTickableGameObject::FRaidBossTickableGameObject()
{
}

FRaidBossTickableGameObject::~FRaidBossTickableGameObject()
{
}

void FRaidBossTickableGameObject::SetIsTickable(bool Tickable)
{
    bIsTickable = Tickable;
}

bool FRaidBossTickableGameObject::IsTickable() const
{
    return bIsTickable;
}

TStatId FRaidBossTickableGameObject::GetStatId() const
{
    return TStatId();
}
