// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_SpawnActorAndFollowCamera.generated.h"

class ARaidBossCharacterBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UAT_SpawnActorAndFollowCamera : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_SpawnActorAndFollowCamera();
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="SpawnActorAndFollowCamera",
			HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAT_SpawnActorAndFollowCamera*	SpawnActorAndFollowCamera(UGameplayAbility* OwningAbility, TSubclassOf<AActor> InSpawnClass,
		const ARaidBossCharacterBase* InParentActor, float Range, FVector InScale = FVector(1, 1, 1), bool bInDoTrackingLocation = true, bool bInFollowRotation = false);

	virtual void	ExternalCancel() override;
protected:
	virtual void	TickTask(float DeltaTime) override;

	virtual void	Activate() override;

	virtual void	OnDestroy(bool bInOwnerFinished) override;
	
public:
/*
*	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
	AActor*	GetSpawnedActor() const { return SpawnedActor; }
	
protected:
	FVector	GetSpawnLocation();
	
protected:
/*
 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	
	//
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	TObjectPtr<AActor>	SpawnedActor;
	
	TWeakObjectPtr<const ARaidBossCharacterBase> ParentActor;

	TSubclassOf<AActor> SpawnClass;
	
	FVector		Scale = {1, 1, 1};

	float		Range = 0;

	FRotator	OriginRotator;

	bool		DoTrackingLocation = true;
	
	bool		bFollowRotation = false;
/*
 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */
	
	//
};
