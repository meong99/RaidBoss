#include "Global/RaidBossGameInstance.h"

#include "Management/RaidBossGameplayTags.h"

URaidBossGameInstance::URaidBossGameInstance()
{
}

void URaidBossGameInstance::Init()
{
    Super::Init();

    RaidBossGameplayTags::InitializeTags();
}
