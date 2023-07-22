#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossItemBase.generated.h"

class ARaidBossCharacterBase;
class URaidBossAbilitySystemComponent;

UENUM(BlueprintType)
enum class EITemCategory : uint8
{
	None UMETA(DisplayName = "None"),
	Equip UMETA(DisplayName = "Equip"),
	Consumable UMETA(DisplayName = "Consumable"),
	Miscellaneous UMETA(DisplayName = "Miscellaneous"),
};

USTRUCT(BlueprintType)
struct FItemAbilityInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EITemCategory					ItemCategory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName							ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UTexture2D>			ItemTexture;
};

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossItemBase : public URaidBossAbilityBase
{
	GENERATED_BODY()
public:
	URaidBossItemBase();
/*
*	----------- Overrided
*/
/*
 *	----------- Binded by Delegate
 */
/*
 *	----------- Other Method
 */
public:
	const TObjectPtr<UTexture2D>	GetItemTexture() const;
	EITemCategory					GetItemCategory() const;
/*
 *	----------- Access
 */
/*
 *	----------- Member Variables
 */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item Base")
	FItemAbilityInformation			ITemInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Item Base")
	TSubclassOf<UGameplayEffect>	ItemEffect;
};
