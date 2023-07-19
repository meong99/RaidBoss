#include "Global/RaidBossInteractionBase.h"

ARaidBossInteractionBase::ARaidBossInteractionBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARaidBossInteractionBase::OnInteration(AActor* Caller)
{
	OnInterationForBlueprint(Caller);
}

void ARaidBossInteractionBase::EndInteraction()
{
	EndInteractionForBlueprin();
}

void ARaidBossInteractionBase::BeginPlay()
{
	Super::BeginPlay();
	
}
