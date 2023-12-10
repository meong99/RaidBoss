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

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	int32	GetEquipType() const;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void	EquipItem();
	
	void	UnEquipItem();

protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	EEquipType EquipType = EEquipType::Size;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveMaxHealth = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveMaxMana = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveAttackPower = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveAttackSpeed = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveAttackRange = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveDefensePower = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Item")
	float	AdditiveMoveSpeed = 0;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	bool bIsThisArmed = false;
};
