// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_SpawnActorAndFollowParent.generated.h"

/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAT_SpawnActorAndFollowParent : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_SpawnActorAndFollowParent();
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="SpawnActorAndFollowParent",
			HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAT_SpawnActorAndFollowParent*	SpawnActorAndFollowParent(UGameplayAbility* OwningAbility, TSubclassOf<AActor> InSpawnClass,
		const ACharacter* InParentActor, float InToMoveForward = 0, FVector InScale = FVector(1, 1, 1), bool bInAttachLocation = true, bool bInFollowRotation = true);

	virtual void ExternalCancel() override;

	AActor*	GetSpawnedActor() const { return SpawnedActor; }

protected:
	
	virtual void TickTask(float DeltaTime) override;

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	TSubclassOf<AActor> SpawnClass;
	
	TWeakObjectPtr<const ACharacter> ParentActor;

	float ToMoveForward = 0;
	
	FVector Scale = {1, 1, 1};
	
	bool bAttachLocation = true;
	
	bool bFollowRotation = true;

	FRotator	OriginRotator;

	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TObjectPtr<AActor>	SpawnedActor;
};
