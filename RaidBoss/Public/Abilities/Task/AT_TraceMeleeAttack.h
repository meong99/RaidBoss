// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_TraceMeleeAttack.generated.h"

/**
 * 
 */

class AWeapon;

// DECLARE_DELEGATE_OneParam(FOnHit, const FHitResult&);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHit, const FHitResult&, HitResult);
UCLASS()
class RAIDBOSS_API UAT_TraceMeleeAttack : public UAbilityTask
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AActor* OwingActor;

	UPROPERTY()
	AWeapon* WeaponActor;

	UPROPERTY()
	UMeshComponent* Mesh;

	FGameplayTagContainer EventTagContainer;
	FDelegateHandle EventHandle;

	float TraceSphereRadius;

	float DrawDebugDuration;

	bool IsTrace;

public:
	UPROPERTY(BlueprintReadWrite)
	FOnHit OnHit;

private:
	UPROPERTY()
	TArray<AActor*> HitArray;

	UPROPERTY()
	TArray<FName> SocketNames;

	UPROPERTY()
	TArray<FVector> LastSocketLocations;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

public:
	UAT_TraceMeleeAttack(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Raid Boss | Ability Task")
	static UAT_TraceMeleeAttack* TraceMeleeAttack(
		UGameplayAbility* _OwningAbility,
		AWeapon* _WeaponActor,
		float _TraceSphereRadius,
		float _DrawDebugDuration);

public:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime);
	virtual void OnDestroy(bool AbilityEnded) override;

public:
	UFUNCTION(BlueprintCallable)
	void HitArrayClear();
	UFUNCTION(BlueprintCallable)
	void	GetReady();
private:
	void Trace(TArray<FHitResult>& _HitResult);
	void Trace(const FVector& _TraceStart, const FVector& _TraceEnd, TArray<FHitResult>& _HitResult);

private:
	void OnGameplayEvent(FGameplayTag _EventTag, const FGameplayEventData* _Payload);
};
