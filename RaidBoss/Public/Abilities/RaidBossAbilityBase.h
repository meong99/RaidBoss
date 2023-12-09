#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/AbilityType.h"
#include "Global/RaidBoss.h"
#include "RaidBossAbilityBase.generated.h"

class URaidBossCharacterStatusAttributeSet;
class URaidBossAbilitySystemComponent;
class ARaidBossCharacterBase;

UCLASS(Abstract)
class RAIDBOSS_API URaidBossAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URaidBossAbilityBase();

	virtual void	OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

/*
*	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
	ECharacterAbilityInputs	GetAbilityInputID() const { return AbilityInputID; };
	FGameplayTag			GetAbilityTriggerTag() const;
	
protected:
	void	SendGameplayEventToActor(AActor* Target, FGameplayTag EventTag, FGameplayEventData Payload);
	
	URaidBossAbilitySystemComponent*			GetOwnerAbilityComponent() const;
	
	const URaidBossCharacterStatusAttributeSet*	GetOwnerCharacterState() const;
	
protected:
	
/*
 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */

	//
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARaidBossCharacterBase>	OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	ECharacterAbilityInputs				AbilityInputID = ECharacterAbilityInputs::None;

	// 변경 필요 - 어레이 말고 몽타주 섹션으로 여러 애니메이션을 선택해서 실행할 수 있도록 하는게 좋을 듯 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Ability base")
	TArray<TObjectPtr<UAnimMontage>>	Montages;
	
/*
 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */

	//
};
