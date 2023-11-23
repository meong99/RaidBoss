#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterTpye.h"
#include "GameplayAbilitySpecHandle.h"
#include "Equipment/EquipManagement.h"
#include "RaidBossCharacterBase.generated.h"

class URaidBossSkillBase;
class	URaidBossAbilityBase;
class	URaidBossAbilitySystemComponent;
class	URaidBossCharacterStatusAttributeSet;
class	UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Peaceful = 0 UMETA(DisplayName = "Peaceful"),
	TwoHandWeapon = 0 UMETA(DisplayName = "TwoHandWeapon"),
	Alive = 1 UMETA(DisplayName = "Alive"),
	CanMove = 2 UMETA(DisplayName = "CanMove"),
	CanUsingAttack = 4 UMETA(DisplayName = "CanUsingAttack"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyNewItemAddedDelegate, URaidBossItemBase*, NewItemCDO, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyItemAmountChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNotifyEquipmentChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, EquipmentType,
	UTexture2D*, NewItemImage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifySkillLevelChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, InCurrentSkillLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyNewWeaponEquippedDelegate, const TArray<URaidBossSkillBase*>& ,NewSkillsg);

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ARaidBossCharacterBase();

	virtual void	Tick(float DeltaSeconds) override;
	virtual void	BeginPlay() override;
	virtual void	PossessedBy(AController* NewController) override;
	virtual void	SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Raid Boss | Character Base")
	void			OnDeath();

	void			MoveCharacter(const FVector2D& Value);
	void			LookCharacter(const FVector2D& Value);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	virtual UAbilitySystemComponent*			GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	URaidBossAbilitySystemComponent*			GetRaidBossAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	const URaidBossCharacterStatusAttributeSet*	GetCharacterStatusAttributeSet() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	double	GetExperience() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	double	GetMaxExperience() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	bool	IsCharacterStateTurnOn(ECharacterState CharacterState) const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	float	GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	float	GetAttackPower() const;
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	void			TurnOnCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	void			TurnOffCharacterStateBitMap(ECharacterState CharacterState);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	virtual void	GiveExperience(double Exp);
	virtual void	CharacterLevelUp(float IncrementNum);


	const FItemAnimations* GetAnimationsByWeaponType(EWeaponType WeaponType) const { return AnimationsByWeaponType.Find(WeaponType); }
	/*
	 *	New
	 */
	UFUNCTION(BlueprintCallable)
	void	EquipWeapon(FWeaponKey WeaponKey);
	UFUNCTION(BlueprintCallable)
	void	UnEquipWeapon();
	UFUNCTION(BlueprintCallable)
	void	Attack() const;
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Character Base")
	void	SetCurrentCharacterState(EPlayerState State);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory")
	int32	DecreaseOrRemoveInventoryData(FGameplayTag InAbilityTriggerTag, bool bRemoveAll = false, int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory")
	int32	IncreaseOrAddInventoryData_ForBlueprint(TSubclassOf<URaidBossItemBase> NewItem = nullptr, int32 Amount = 1);
	int32	IncreaseOrAddInventoryData(URaidBossItemBase* NewItem = nullptr, int32 Amount = 1);
	void	RemoveAbilityData(FGameplayTag InAbilityTriggerTag);
	
	void	SetCurrentPlayerState(const EPlayerState InPlayerState) { CurrentPlayerState = InPlayerState; }
	
	FCharacterAnimations	GetCharacterAnimations() const { return CharacterAnimations; }
	EPlayerState			GetCurrentPlayerState() const { return CurrentPlayerState; }
	float					GetMovementMagnitude() const { return MovementMagnitude; }
	FVector					GetMovementDirection() const { return MovementDirection; }
	const TMap<FGameplayTag, FInventoryData>&	GetInventoryData() const { return InventoryData; }
	int32					GetItemAmount(FGameplayTag InAbilityTriggerTag) const;
	const TArray<TSubclassOf<URaidBossSkillBase>>*	GetPlayerSkills() const { return &PlayerSkills; }
	float	GetInputMoveForward() const { return InputMoveForward; }
	float	GetInputMoveRight() const { return InputMoveRight; }
	bool	IsDuringAlign() const { return bIsDuringAlign; }
	bool	IsTurnLeft() const { return bIsTurnLeft; }

	UFUNCTION(BlueprintCallable)
	bool	IsMovementBlocked() const { return bIsMovementBlocked; }
	bool	CanActivateNormalAttack() const { return bCanActivateNormalAttack; }

	void	SetIsTurnLeft(bool bInIsTurnLeft) { bIsTurnLeft = bInIsTurnLeft; }
	void	SetIsDuringAlign(bool bInIsDuringAlign) { bIsDuringAlign = bInIsDuringAlign; }
	UFUNCTION(BlueprintCallable)
	void	SetIsMovementBlocked(bool bInIsMovementBlocked) { bIsMovementBlocked = bInIsMovementBlocked; }
	void	SetCanActivateNormalAttack(bool	bInCanActivateNormalAttack) { bCanActivateNormalAttack = bInCanActivateNormalAttack; }
	
protected:
	
	bool	IsItEquipment(const URaidBossItemBase* NewItem) const;
	
	void	GiveDefaultAbilities() const;

	void	GiveAndActivateAbilities() const;
	
	void	ApplyCharacterStatusEffect() const;
	
	void	SetAnimationData(const UDataTable* AnimDataTable);
	
	void	AddNewItemData(URaidBossItemBase* NewItem, FInventoryData& Data);

	void	GiveSkillAbilities();
public:

	UPROPERTY(BlueprintAssignable ,BlueprintReadWrite, Category="Raid Boss | Delegate")
	FNotifyNewItemAddedDelegate			NotifyNewItemAdded;
	UPROPERTY(BlueprintAssignable ,BlueprintReadWrite, Category="Raid Boss | Delegate")
	FNotifyItemAmountChangedDelegate	NotifyItemAmountChanged;
	UPROPERTY(BlueprintAssignable ,BlueprintReadWrite, Category="Raid Boss | Delegate")
	FNotifyEquipmentChangedDelegate		NotifyEquipmentChanged;
	UPROPERTY(BlueprintAssignable ,BlueprintReadWrite, Category="Raid Boss | Delegate")
	FNotifySkillLevelChangedDelegate	NotifySkillLevelChanged;
	UPROPERTY(BlueprintAssignable ,BlueprintReadWrite, Category="Raid Boss | Delegate")
	FNotifyNewWeaponEquippedDelegate	NotifyNewWeaponEquipped;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Character Base")
	TObjectPtr<URaidBossAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Base")
	const URaidBossCharacterStatusAttributeSet*	CharacterStatusAttributeSet;
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TSubclassOf<UGameplayEffect>				CharacterStatusEffect;
	
	int32	CharacterStateBitMask;
	double	Experience = 0;// 경험치 어트리뷰트로 ㄱㄱ
	double	MaxExperience = 100;
	float	CharacterLevel = 1;



	/*
	 *	New
	 */
	UPROPERTY()
	UEquipManagement*	EquipManager;  
	UPROPERTY(BlueprintReadWrite, Category = "Raid Boss | Weapon")
	EWeaponType	CurrentEquippedWeaponType;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon")
	TObjectPtr<AWeapon>	EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Weapon | Animations")
	FCharacterAnimations	CharacterAnimations;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon | Animations")
	TMap<EWeaponType, FItemAnimations>	AnimationsByWeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Raid Boss | Character Base", meta=(AllowPrivateAccess))
	EPlayerState	CurrentPlayerState = EPlayerState::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Character Movement", meta=(AllowPrivateAccess))
	float	MovementMagnitude;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Character Movement", meta=(AllowPrivateAccess))
	FVector	MovementDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Raid Boss | Item")
	TMap<FGameplayTag, FInventoryData>				InventoryData;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Item")
	TMap<FGameplayTag, FGameplayAbilitySpecHandle>	OwningItemSpecHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill");
	TArray<TSubclassOf<URaidBossSkillBase>>			PlayerSkills;

	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultActivateAbilities;

	
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon")
	float InputMoveForward = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon")
	float InputMoveRight = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon")
	bool bIsDuringAlign = false;
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon")
	bool bIsTurnLeft = false;

	bool	bIsMovementBlocked = false;
	UPROPERTY(BlueprintReadWrite, Category = "Raid Boss | Weapon")
	bool	bCanActivateNormalAttack = true;
};
