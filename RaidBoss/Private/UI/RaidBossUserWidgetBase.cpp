#include "UI/RaidBossUserWidgetBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"

TSet<URaidBossUserWidgetBase*>	URaidBossUserWidgetBase::WidgetsInViewport = TSet<URaidBossUserWidgetBase*>();

URaidBossUserWidgetBase::URaidBossUserWidgetBase(const FObjectInitializer& Initializer) : Super(Initializer)
{
	WidgetsInViewport.Reset();
}

void URaidBossUserWidgetBase::AddToViewportWithTracking(int32 ZOrder)
{
	bool	bIsAlreadyInSet = false;
	
	WidgetsInViewport.Add(this, &bIsAlreadyInSet);
	
	if (bIsAlreadyInSet == false)
		AddToViewport(ZOrder);
}

void URaidBossUserWidgetBase::RemoveFirstAddedWidget(int32& OutRemainingWidget)
{
	if (WidgetsInViewport.IsEmpty() == false)
	{
		URaidBossUserWidgetBase* WidgetToRemove = *WidgetsInViewport.begin();

		WidgetsInViewport.Remove(WidgetToRemove);
		WidgetToRemove->RemoveFromParent();
	}
	OutRemainingWidget = WidgetsInViewport.Num();
}

void URaidBossUserWidgetBase::RemoveWidget(URaidBossUserWidgetBase* WidgetToRemove, int32& OutRemainingWidget)
{
	if (WidgetsInViewport.Find(WidgetToRemove) != nullptr)
	{
		WidgetToRemove->RemoveFromParent();
		WidgetsInViewport.Remove(WidgetToRemove);
	}
	OutRemainingWidget = WidgetsInViewport.Num();
}

void URaidBossUserWidgetBase::RemoveAllWidget()
{
	if (WidgetsInViewport.IsEmpty() == false)
	{
		for (const auto& Element : WidgetsInViewport)
		{
			Element->RemoveFromParent();
		}

		WidgetsInViewport.Reset();
		
		ARaidBossPlayerControllerBase* PlayerControllerBase = Cast<ARaidBossPlayerControllerBase>(GetOwningPlayer());
	
		if (PlayerControllerBase)
		{
			FInputModeGameOnly	InputMode;
			
			PlayerControllerBase->SetInputMode(InputMode);
			PlayerControllerBase->SetShowMouseCursor(false);
		}
	}
}
