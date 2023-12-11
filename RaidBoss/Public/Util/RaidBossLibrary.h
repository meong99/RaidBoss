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
    /**
     * 해당 로케이션과 방향 기준으로 일정 각도 내에 있는 FilterClass의 액터들을 반환.
     * 
     * @param Location			검사할 원의 중심 Location
     * @param Direction			Location + Direction * Distance를 기준으로 좌우 Angle / 2만큼의 각도를 검사함 (왼쪽 오른쪽의 합이 Angle)
     * @param Radius			검사할 원의 반지름
     * @param FilterClasses		검출하고자 하는 Actor의 클래스들
     * @param IgnoreClasses		검출하지 않고자 하는 Actor의 클래스들
     * @param bIgnoreDistanceZ	Z축을 무시하고 검사함
     * 
     * @return 검출된 액터들
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss Library",
        meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext, AutoCreateRefTerm="IgnoreClasses"))
    static TArray<AActor*> GetActorsInAngle(const UObject* WorldContextObject, const FVector& Location,
                                            const FVector& Direction, float Radius,
                                            const TArray<TSubclassOf<AActor>>& FilterClasses,
                                            const TArray<TSubclassOf<AActor>>& IgnoreClasses,
                                            float Angle = 45, bool bIgnoreDistanceZ = true, bool DrawDebug = false);

    /**
     * Location을 중심으로 직사각형의 범위 내의 액터들을 반환
     * 
     * @param Location			검사 시작점
     * @param Direction			검사를 진행할 방향
     * @param ForwardDistance	Direction * ForwardDistance만큼 검사. 검사할 방향의 거리
     * @param SideDistance		Location 기준 좌우 SideDistance/2 만큼을 검사함 (왼쪽, 오른쪽 합이 SideDistance)
     * @param FilterClasses		검출하고자 하는 Actor의 클래스들
     * @param IgnoreClasses		검출하지 않고자 하는 Actor의 클래스들
     * @param bIgnoreDistanceZ	Z축을 무시하고 검사함
     * 
     * @return 검출된 액터들
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss Library",
        meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext, AutoCreateRefTerm="IgnoreClasses"))
    static TArray<AActor*> GetActorsInRectangle(const UObject* WorldContextObject, const FVector& Location,
                                                const FVector& Direction, float ForwardDistance,
                                                float SideDistance, const TArray<TSubclassOf<AActor>>& FilterClasses,
                                                const TArray<TSubclassOf<AActor>>& IgnoreClasses,
                                                bool bIgnoreDistanceZ = true, bool DrawDebug = false);

    /**
     * 로케이션 위치에서 바닥 검출.
     * 
     * @param StartLocation	검출 시작점
     * @param TraceLength	FVector{0, 0, -1}의 방향으로 검사를 하는데 아래로 검사할 거리
     * 
     * @return 검사 결과(바닥 검출 성공하면 바닥 Location, 실패하면 StartLocation
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss Library",
        meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext))
    static FVector GetFloorLocation(const UObject* WorldContextObject, const FVector& StartLocation,
                                    float TraceLength = 3000, bool bDrawDebugLine = false);

    /**
     * 카메라 Location -> Camera ForwardVector로 라인 트레이싱
     * 
     * @param CharacterBase			카메라를 보유하고있는 캐릭터
     * @param OutTraceEndLocation	트레이싱 실패 시 트레이싱 End Location을 반환하기 위해 존재
     * 
     * @return 검출된 액터들
     */
    UFUNCTION(BlueprintCallable, Category="Raid Boss Library",
        meta = (WorldContext="WorldContextObject", CallableWithoutWorldContext))
    static bool LineTraceForObjectFromCamera(const UObject* WorldContextObject,
                                             const ARaidBossCharacterBase* CharacterBase,
                                             const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
                                             const TArray<AActor*>& ActorsToIgnore,
                                             FHitResult& OutHit, FVector& OutTraceEndLocation, bool bIgnoreSelf,
                                             float TraceLength = 1000, bool bDrawDebugLine = false);

    /*
     * 블루프린트 라이브러리들 편하게 호출하기 위해 존재. * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    */

    //
    static bool GetVelocityToLaunchActor(const UObject* WorldContextObject, FVector& OutLaunchVelocity,
                                         FVector StartPos, FVector EndPos, float ArcParam = 0.5f,
                                         float OverrideGravityZ = 0);

    static UParticleSystemComponent* SpawnEmitterAtLocation(UWorld* World, UParticleSystem* EmitterTemplate,
                                                            const FTransform& SpawnTransform, bool bAutoDestroy = true,
                                                            EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None,
                                                            bool bAutoActivate = true);

    static FVector RotateVector(FVector Vector, FRotator Rotator);
};
