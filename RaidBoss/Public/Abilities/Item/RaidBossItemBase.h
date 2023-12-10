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
	int32					BuyingPrice;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32					SellingPrice;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D>	ItemTexture;
	
	UPROPERTY(BlueprintReadOnly)
	uint8					ItemKey = INDEX_NONE;
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class RAIDBOSS_API URaidBossItemBase : public URaidBossAbilityBase
{
	GENERATED_BODY()

public:
	
	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	UTexture2D*				GetItemTexture() const { return ITemInfo.ItemTexture; }
	EITemCategory			GetItemCategory() const { return ITemInfo.ItemCategory; }
	FItemAbilityInformation	GetItemInfo() const { return ITemInfo; }
	uint8					GetItemKey() const { return ITemInfo.ItemKey; }
	
	void	SetItemKey(uint8 ItemKey) { ITemInfo.ItemKey = ItemKey; }
	
protected:
		
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	FItemAbilityInformation			ITemInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	TSubclassOf<UGameplayEffect>	ItemEffect;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
};
