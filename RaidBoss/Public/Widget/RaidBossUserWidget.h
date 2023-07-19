#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "RaidBossUserWidget.generated.h"

class URaidBossAbilityBase;

UCLASS()
class RAIDBOSS_API URaidBossUserWidget : public UUserWidget
{
	GENERATED_BODY()
	/*
	 *	----------- Overrided
	 */
	/*
	 *	----------- Binded by Delegate
	 */
	/*
	 *	----------- Other Method
	 */
	/*
	 *	----------- Access
	 */
public:
	void	AddAbility(IN URaidBossAbilityBase* InAbilityArray);
	/*
	 *	----------- Member Variables
	 */
protected:
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TArray<URaidBossAbilityBase*>	AbilityArray;
};
