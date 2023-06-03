#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "RaidBossItemBase.generated.h"

UCLASS(Blueprintable)
class RAIDBOSS_API URaidBossItemBase : public UObject
{
	GENERATED_BODY()
public:
	URaidBossItemBase();
	UFUNCTION(BlueprintCallable)
	void ConstructedByOwner(AActor* Owner);

	UFUNCTION(BlueprintCallable)
		TArray<FGameplayEffectSpecHandle>	GetSpecHandle() const { return SpecHandles; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName									ItemCategory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName									ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UTexture2D>					ItemTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<UGameplayEffect>>	ItemEffects;
	UPROPERTY()
		TArray<FGameplayEffectSpecHandle>		SpecHandles;
	UPROPERTY()
		TObjectPtr<AActor>						OwnerActor;
};
