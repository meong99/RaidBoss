#pragma once

#include "CoreMinimal.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "RaidBossItemBase.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EITemCategory			ItemCategory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName					ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D>	ItemTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32					BuyingPrice;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32					SellingPrice;
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class RAIDBOSS_API URaidBossItemBase : public URaidBossAbilityBase
{
	GENERATED_BODY()

public:
	UTexture2D*				GetItemTexture() const;
	EITemCategory			GetItemCategory() const;
	FItemAbilityInformation	GetItemInfo() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item Base")
	FItemAbilityInformation			ITemInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Consumable Item")
	TSubclassOf<UGameplayEffect>	ItemEffect;
};
