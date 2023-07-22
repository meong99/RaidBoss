#pragma once

#include "CoreMinimal.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "RaidBossEquipmentItem.generated.h"

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	Helmet UMETA(DisplayName = "Hat"),
	Weapon UMETA(DisplayName = "Weapon"),
	ArmorTop UMETA(DisplayName = "Top"),
	ArmorBottom UMETA(DisplayName = "Bottom"),
	ArmorShoes UMETA(DisplayName = "Shoes"),
	Size UMETA(DisplayName = "Size"),
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
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
/*
*	----------- Binded by Delegate
*/
/*
*	----------- Other Method
*/
protected:
	void	EquipItem();
	void	UnEquipItem();
/*
*	----------- Access
*/
public:
	int32	GetEquipType() const;
/*
*	----------- Static Variables
*/
/*
*	----------- Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Equipment Item")
	EEquipType EquipType = EEquipType::Size;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Equipment Item")
	TArray<FGameplayModifierInfo>				ItemModifiers;
};
