#pragma once

#include "Engine/DataTable.h"
#include "EquipmentType.generated.h"

class UGameplayEffect;
class URaidBossAbilityBase;

UENUM(BlueprintType)
enum class EWeaponAttachSocket : uint8
{
    WeaponSocket_R UMETA(DisplayName = "WeaponSocket_R"),
    WeaponSocket_L UMETA(DisplayName = "WeaponSocket_L"),
    WeaponSocket_R2 UMETA(DisplayName = "WeaponSocket_R2"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None UMETA(DisplayName = "None"),

    TwoHandSword UMETA(DisplayName = "TwoHandSword"),
    Bow UMETA(DisplayName = "Bow"),
    NoWeapon UMETA(DisplayName = "NoWeapon"),

    Monster UMETA(DisplayName = "Monster"),
    Player UMETA(DisplayName = "Player"),

    LastIndex UMETA(DisplayName = "LastIndex"),
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
    None UMETA(DisplayName = "None"),
};

USTRUCT(BlueprintType)
struct FWeaponKey
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EWeaponType WeaponType;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 WeaponNumber;

    bool operator==(const FWeaponKey& Other) const
    {
        if (Other.WeaponType == WeaponType &&
            Other.WeaponNumber == WeaponNumber)
        {
            return true;
        }

        return false;
    }

    bool operator!=(const FWeaponKey& Other) const
    {
        return !(Other == *this);
    }
};

USTRUCT()
struct FArmorKey
{
    GENERATED_BODY()

    EArmorType ArmorType;
    int32 ArmorNumber;
};

USTRUCT(BlueprintType)
struct FItemAnimations
{
    GENERATED_BODY()

public:
    UAnimMontage* GetEquipAnims() { return EquipAnim; }
    UAnimMontage* GetUnEquipAnims() { return UnEquipAnim; }
    TArray<UAnimMontage*> GetUseAnims() { return UseAnims; }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* EquipAnim;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* UnEquipAnim;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UAnimMontage*> UseAnims;
};

USTRUCT(BlueprintType)
struct FAnimationsByWeapon : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EWeaponType WeaponType;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FItemAnimations AnimationsByWeaponType;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FWeaponKey WeaponKey;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FString WeaponName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float AttackPower;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float AttackRange;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> SkeletalMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<TSubclassOf<URaidBossAbilityBase>> Abilities;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FItemAnimations WeaponAnimations;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EWeaponAttachSocket AttachSocket;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UGameplayEffect> AdditiveEffectToCharacter;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<TSubclassOf<UUserWidget>> WidgetsToApply;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<UParticleSystem> BasicAttackParticle;
};
