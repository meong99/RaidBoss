#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaidBossUserWidgetBase.generated.h"

UCLASS()
class RAIDBOSS_API URaidBossUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	URaidBossUserWidgetBase(const FObjectInitializer& Initializer);

/*
*	----------- Override
*/
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | User Widget Base")
	void AddToViewportWithTracking(int32 ZOrder = 0);
/*
*	----------- Used on delegate
*/
/*
*	----------- Process Method
*/
public:
	UFUNCTION(BlueprintCallable, Category="Raid Boss | User Widget Base")
	void	RemoveFirstAddedWidget(int32& OutRemainingWidget);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | User Widget Base")
	void	RemoveWidget(URaidBossUserWidgetBase* WidgetToRemove, int32& OutRemainingWidget);
	UFUNCTION(BlueprintCallable, Category="Raid Boss | User Widget Base")
	void	RemoveAllWidget();
/*
*	----------- Access(Get, Set, Check)
*/
/*
*	----------- Member Variables
*/
private:
	static TSet<URaidBossUserWidgetBase*>	WidgetsInViewport;
};
