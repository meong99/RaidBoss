// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RaidBossLibrary.generated.h"

class ARaidBossCharacterBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API URaidBossLibrary : public UObject
{
	GENERATED_BODY()

public:
	// 해당 로케이션과 방향 기준으로 일정 각도 내에 있는 FilterClass의 액터들을 반환.
	UFUNCTION(BlueprintCallable, Category="Raid Boss Library", meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext, AutoCreateRefTerm="IgnoreClasses"))
	static TArray<AActor*>	GetActorsInAngle(const UObject* WorldContextObject, const FVector& Location, const FVector& Direction, float Distance, 
		const TArray<TSubclassOf<AActor>>& FilterClasses,
		const TArray<TSubclassOf<AActor>>& IgnoreClasses,
		float Angle = 45, bool bIgnoreDistanceZ = true, bool DrawDebug = false);

	UFUNCTION(BlueprintCallable, Category="Raid Boss Library", meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext, AutoCreateRefTerm="IgnoreClasses"))
	static TArray<AActor*>	GetActorsInRectangle(const UObject* WorldContextObject, const FVector& Location, const FVector& Direction, float ForwardDistance, 
		float SideDistance, const TArray<TSubclassOf<AActor>>& FilterClasses, const TArray<TSubclassOf<AActor>>& IgnoreClasses,
		bool bIgnoreDistanceZ = true, bool DrawDebug = false);

	// 로케이션 위치에서 바닥 검출. 검출 실패시 StartLocation 반환
	UFUNCTION(BlueprintCallable, Category="Raid Boss Library", meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext))
	static FVector	GetFloorLocation(const UObject* WorldContextObject, const FVector& StartLocation, float TraceLength = 3000, bool bDrawDebugLine = false);

	//카메라의 시점으로부터 라인 트레이싱
	UFUNCTION(BlueprintCallable, Category="Raid Boss Library", meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext))
	static bool	LineTraceForObjectFromCamera(const UObject* WorldContextObject, const ARaidBossCharacterBase* CharacterBase,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore,
		FHitResult& OutHit, FVector& OutTraceEndLocation, bool bIgnoreSelf, float TraceLength = 1000, bool bDrawDebugLine = false);
	
	// 블루프린트 라이브러리들 편하게 호출하기 위해 존재.

	static bool	GetVelocityToLaunchActor(const UObject* WorldContextObject, FVector& OutLaunchVelocity, FVector StartPos, FVector EndPos, float ArcParam = 0.5f, float OverrideGravityZ = 0);
	
	static UParticleSystemComponent* SpawnEmitterAtLocation(UWorld* World, UParticleSystem* EmitterTemplate,
		const FTransform& SpawnTransform, bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None,
		bool bAutoActivate = true);
	
	static FVector	RotateVector(FVector Vector, FRotator Rotator);
};
