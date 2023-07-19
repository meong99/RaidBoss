#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "RaidBossEquipmentItem.generated.h"

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	Hat UMETA(DisplayName = "Hat"),
	Weapon UMETA(DisplayName = "Weapon"),
	Top UMETA(DisplayName = "Top"),
	Bottom UMETA(DisplayName = "Bottom"),
	Shoes UMETA(DisplayName = "Shoes"),
	None UMETA(DisplayName = "None"),
};

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossEquipmentItem : public URaidBossItemBase
{
	GENERATED_BODY()
public:
	URaidBossEquipmentItem();
/*
*	----------- Overrided
*/
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
protected:
	void	EquipItem();
	void	UnEquipItem();
	void	RemoveAppliedItemEffect();
/*
*	----------- Access
*/
public:
	bool	IsEquipped() const;
	int32	GetEquipType() const;
	static	const TMap<EEquipType, URaidBossEquipmentItem*>&	GetEquippedItems();
/*
*	----------- Static Variables
*/
protected:
	static TMap<EEquipType, URaidBossEquipmentItem*>	EquippedItems;
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipType EquipType = EEquipType::None;

	TArray<FActiveGameplayEffectHandle>	EffectHandleArray;
};
