#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "UI/InteractionalUISystem.h"

ARaidBossPlayerControllerBase::ARaidBossPlayerControllerBase()
{
    InteractionalUISystem = CreateDefaultSubobject<UInteractionalUISystem>(TEXT("Interactional UI System"));
}

void ARaidBossPlayerControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (InteractionalUISystem)
    {
        InteractionalUISystem->InteractionalUIParsing(InteractionalUIArray);
    }
}

void ARaidBossPlayerControllerBase::MoveCharacter(FVector2D Value) const
{
    ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

    if (PlayerBase)
    {
        PlayerBase->MoveCharacter(Value);
    }
}

void ARaidBossPlayerControllerBase::JumpCharacter() const
{
    ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

    if (PlayerBase)
    {
        PlayerBase->JumpCharacter();
    }
}

void ARaidBossPlayerControllerBase::StopJumpCharacter() const
{
    ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

    if (PlayerBase)
    {
        PlayerBase->StopJumpCharacter();
    }
}

void ARaidBossPlayerControllerBase::LookCharacter(FVector2D Value) const
{
    ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

    if (PlayerBase)
    {
        PlayerBase->LookCharacter(Value);
    }
}

void ARaidBossPlayerControllerBase::SendUIEventTagToController(FGameplayTag TriggerTag) const
{
    if (InteractionalUISystem)
    {
        int32 AddedUI = InteractionalUISystem->HandleUITriggerEvent(TriggerTag);
    }
}

URaidBossAbilitySystemComponent* ARaidBossPlayerControllerBase::GetRaidBossAbilitySystemComponent() const
{
    ARaidBossPlayerBase* PlayerBase = GetRaidBossPlayerBase();

    return PlayerBase ? PlayerBase->GetRaidBossAbilitySystemComponent() : nullptr;
}

ARaidBossPlayerBase* ARaidBossPlayerControllerBase::GetRaidBossPlayerBase() const
{
    return Cast<ARaidBossPlayerBase>(GetPawn());
}
