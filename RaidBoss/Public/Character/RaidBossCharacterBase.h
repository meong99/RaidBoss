#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterTpye.h"
#include "GameplayAbilitySpecHandle.h"
#include "GenericTeamAgentInterface.h"
#include "Camera/CameraComponent.h"
#include "Equipment/EquipManagement.h"
#include "GameFramework/SpringArmComponent.h"
#include "RaidBossCharacterBase.generated.h"

class URaidBossSkillBase;
class URaidBossAbilityBase;
class URaidBossAbilitySystemComponent;
class URaidBossCharacterStatusAttributeSet;
class UGameplayEffect;

#define MONSTER_TEAM_ID 1
#define PLAYER_TEAM_ID 0

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyNewItemAddedDelegate, URaidBossItemBase*, NewItemCDO, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyItemAmountChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNotifyEquipmentChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, EquipmentType,
	UTexture2D*, NewItemImage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifySkillLevelChangedDelegate, FGameplayTag, InAbilityTriggerTag, int32, InCurrentSkillLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyNewWeaponEquippedDelegate, const TArray<URaidBossSkillBase*>& ,NewSkillsg);

UCLASS(Abstract)
class RAIDBOSS_API ARaidBossCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ARaidBossCharacterBase();

	virtual void	PossessedBy(AController* NewController) override;
	virtual void	SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void	BeginPlay() override;
	virtual void	Tick(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void	EquipWeapon(FWeaponKey WeaponKey);
	
	UFUNCTION(BlueprintCallable)
	void	UnEquipWeapon();
	
	UFUNCTION(BlueprintCallable)
	void	Attack() const;
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory")
	int32	DecreaseOrRemoveInventoryData(FGameplayTag InAbilityTriggerTag, bool bRemoveAll = false, int32 Amount = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Raid Boss | Inventory")
	int32	IncreaseOrAddInventoryData(uint8 ItemKey, int32 Amount = 1);

	void	MoveCharacter(const FVector2D& Value);
	
	void	LookCharacter(const FVector2D& Value);

	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	virtual UAbilitySystemComponent*				GetAbilitySystemComponent() const override;
	URaidBossAbilitySystemComponent*				GetRaidBossAbilitySystemComponent() const;
	const URaidBossCharacterStatusAttributeSet*		GetCharacterStatusAttributeSet() const;
	UCameraComponent*								GetFollowCamera() const { return FollowCamera; };
	const TMap<FGameplayTag, FInventoryData>&		GetInventoryData() const { return InventoryData; }
	const TArray<TSubclassOf<URaidBossSkillBase>>*	GetPlayerSkills() const { return &PlayerSkills; }
	FCharacterAnimations	GetCharacterAnimations() const { return CharacterAnimations; }
	ECharacterState			GetCurrentCharacterState() const { return CurrentCharacterState; }
	int32					GetItemAmount(FGameplayTag InAbilityTriggerTag) const { return InventoryData.FindRef(InAbilityTriggerTag).Amount; };
	float					GetInputMoveForward() const { return InputMoveForward; }
	float					GetInputMoveRight() const { return InputMoveRight; }
	bool					IsDuringAlign() const { return bIsDuringAlign; }
	bool					IsTurnLeft() const { return bIsTurnLeft; }
	bool					IsMovementBlocked() const { return bIsMovementBlocked; }
	bool					CanActivateNormalAttack() const { return bCanActivateNormalAttack; }
	virtual FGenericTeamId	GetGenericTeamId() const override { return TeamID; }
	float					GetGold() const { return Gold; }

	virtual void	SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamID = NewTeamID; };
	void			SetCurrentCharacterState(ECharacterState State) { CurrentCharacterState = State; };
	void			SetCurrentPlayerState(const ECharacterState InPlayerState) { CurrentCharacterState = InPlayerState; }
	void			SetIsTurnLeft(bool bInIsTurnLeft) { bIsTurnLeft = bInIsTurnLeft; }
	void			SetIsDuringAlign(bool bInIsDuringAlign) { bIsDuringAlign = bInIsDuringAlign; }
	void			SetIsMovementBlocked(bool bInIsMovementBlocked) { bIsMovementBlocked = bInIsMovementBlocked; }
	void			SetCanActivateNormalAttack(bool	bInCanActivateNormalAttack) { bCanActivateNormalAttack = bInCanActivateNormalAttack; }

protected:
	void	ApplyCharacterDefaultSpecEffectToSelf();

	bool	IsItEquipment(const URaidBossItemBase* NewItem) const;
	
	void	GiveDefaultAbilities() const;

	void	GiveAndActivateAbilities() const;
	
	void	InitAnimationData(const UDataTable* AnimDataTable);
	
	void	AddNewItemData(URaidBossItemBase* NewItem, OUT FInventoryData& OutData, uint8 ItemKey);

	void	GiveSkillAbilities();

	// 부여된 아이템 어빌리티 제거
	void	RemoveItemAbility(FGameplayTag InAbilityTriggerTag);
	
public:

	/*
	 *	Delegate * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
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

	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Character Spec")
	FCharacterSpec								CharacterDefaultSpec;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Weapon | Animations")
	FCharacterAnimations						CharacterAnimations; // 변경 필요 - 기본적인 애니메이션은 애니메이션쪽에서 관리하자. 무기가 있으니 무기도 관리하고
	
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TSubclassOf<UGameplayEffect>				CharacterStatusEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultAbilities;
	
	// 부여하자마자 실행할 어빌리티들
	UPROPERTY(EditDefaultsOnly, Category = "Raid Boss | Character Base")
	TArray<TSubclassOf<URaidBossAbilityBase>>	DefaultActivateAbilities;

	//플레이어가 배울 수 있는 스킬들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Skill", SaveGame);
	TArray<TSubclassOf<URaidBossSkillBase>>		PlayerSkills;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Character Base", meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent>				CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Raid Boss | Character Base", meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent>				FollowCamera;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Raid Boss | Character Base")
	TObjectPtr<URaidBossAbilitySystemComponent>	AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Character Base", SaveGame)
	const URaidBossCharacterStatusAttributeSet*	CharacterStatusAttributeSet;
	
	UPROPERTY()
	TObjectPtr<UDataTable>	ItemDataTable;
	
	UPROPERTY()
	UEquipManagement*		EquipManager;  
	
	// 플레이어와 몬스터를 구분지을 수 있는 팀
	FGenericTeamId	TeamID;

	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	// 플레이어가 소지하고있는 아이템들
	UPROPERTY(BlueprintReadWrite, Category = "Raid Boss | Item", SaveGame)
	TMap<FGameplayTag, FInventoryData>				InventoryData;

	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon", SaveGame)// 변경 예상 - 게임 저장할 때 타입만 할 수도?
	TObjectPtr<AWeapon>								EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Weapon | Animations")
	TMap<EWeaponType, FItemAnimations>				AnimationsByWeaponType; // 변경 필요 - 무기 자체에 애니메이션 추가해서 사용
	
	// 아이템들에대한 어빌리티 스펙들
	UPROPERTY(BlueprintReadOnly, Category = "Raid Boss | Item")
	TMap<FGameplayTag, FGameplayAbilitySpecHandle>	OwningItemSpecHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Raid Boss | Character Base")
	ECharacterState	CurrentCharacterState = ECharacterState::None; // 변경 예상 - 뭔가 깔끔하게 상태 변경이 필요할수도?
	
	UPROPERTY(BlueprintReadWrite, Category = "Raid Boss | Weapon", SaveGame)
	EWeaponType		CurrentEquippedWeaponType; // 변경 필요 - 무기 자체가 아이덴티티가 됨. 타입 말고 이름같은걸로?
	
	//플레이어 소지 골드
	UPROPERTY(BlueprintReadWrite, Category = "Raid Boss | Item", SaveGame)
	float	Gold = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss")
	bool	bIsMovementBlocked = false;

	// 플레이어 기본 공격
	UPROPERTY(BlueprintReadWrite, Category="Raid Boss")
	bool	bCanActivateNormalAttack = true;

	// 플레이어 움직임 및 회전
	float	InputMoveForward = 0;
	
	float	InputMoveRight = 0;
	
	bool	bIsDuringAlign = false;
	
	bool	bIsTurnLeft = false;
};
