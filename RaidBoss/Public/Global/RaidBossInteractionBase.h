#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaidBossInteractionBase.generated.h"

UCLASS()
class RAIDBOSS_API ARaidBossInteractionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARaidBossInteractionBase();

/*
 *	----------- Overrided
 */
protected:
	virtual void	BeginPlay() override;
	
/*
 *	----------- Binded by Delegate
 */
	
/*
 *	----------- Other Method
 */
public:	
	virtual void	OnInteration(AActor* Caller);
	virtual void	EndInteraction();
	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | Interation Base")
	void			OnInterationForBlueprint(AActor* Caller);
	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | Interation Base")
	void			EndInteractionForBlueprin();
	
/*
 *	----------- Access
 */
/*
 *	----------- Member Variables
 */
};
