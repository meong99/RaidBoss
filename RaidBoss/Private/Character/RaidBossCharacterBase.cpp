#include "Character/RaidBossCharacterBase.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Equipment/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARaidBossCharacterBase::ARaidBossCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());
	
	AbilitySystemComponent = CreateDefaultSubobject<URaidBossAbilitySystemComponent>(TEXT("Ability Component"));
	CharacterStatusAttributeSet = CreateDefaultSubobject<URaidBossCharacterStatusAttributeSet>(TEXT("Character Status AttributeSet"));
	EquipManager = CreateDefaultSubobject<UEquipManagement>(TEXT("Equip Manager"));
	
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::Alive);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanMove);
	CharacterStateBitMask |= static_cast<int32>(ECharacterState::CanUsingAttack);
}

void ARaidBossCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AbilitySystemComponent && bIsMovementBlocked == false)
	{
		AbilitySystemComponent->PressInputID(static_cast<int32>(ECharacterAbilityInputs::Look));
	}
}

void ARaidBossCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	GiveDefaultAbilities();
	GiveAndActivateAbilities();
	GiveSkillAbilities();
}

void ARaidBossCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (EquippedWeapon)
	{
		EquippedWeapon->NotifyNewWeaponEquipped();
	}
}

void ARaidBossCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const FGameplayAbilityInputBinds Binds = {
		FString("ConfirmTarget"), FString("CancelTarget"),
		FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("ECharacterAbilityInputs"))),
		static_cast<int32>(ECharacterAbilityInputs::Confirm), static_cast<int32>(ECharacterAbilityInputs::Cancel)
	};
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);
}

void ARaidBossCharacterBase::MoveCharacter(const FVector2D& Value)
{
	FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	FVector Right = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

	InputMoveForward = Value.Y;
	MovementDirection = Forward;
	MovementMagnitude = Value.Y;
	if (AbilitySystemComponent && bIsMovementBlocked == false)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(ECharacterAbilityInputs::Move));
	}

	InputMoveRight = Value.X;
	MovementDirection = Right;
	MovementMagnitude = Value.X;
	if (AbilitySystemComponent && bIsMovementBlocked == false)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(ECharacterAbilityInputs::Move));
	}
}

void ARaidBossCharacterBase::LookCharacter(const FVector2D& Value)
{
	AddControllerYawInput(Value.X * 0.4);
	AddControllerPitchInput(Value.Y * 0.4);
}

int32 ARaidBossCharacterBase::GetItemAmount(FGameplayTag InAbilityTriggerTag) const
{
	return InventoryData.FindRef(InAbilityTriggerTag).Amount;
}

bool ARaidBossCharacterBase::IsItEquipment(const URaidBossItemBase* NewItem) const
{
	if (NewItem == nullptr)
	{
		return false;
	}
	
	return NewItem->GetItemCategory() == EITemCategory::Equip;
}

void ARaidBossCharacterBase::GiveDefaultAbilities() const
{
	if (AbilitySystemComponent == nullptr)
		return;
	
	for (const auto Ability : DefaultAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpecHandle	AbilitySpecHandle;
			URaidBossAbilityBase*		AbilityCDO = Cast<URaidBossAbilityBase>(Ability->GetDefaultObject());
			
			AbilitySystemComponent->GiveAbilityWithoutDuplication(Ability, AbilitySpecHandle, static_cast<int32>(AbilityCDO->GetAbilityInputID()));
		}
	}
}

void ARaidBossCharacterBase::GiveAndActivateAbilities() const
{
	if (AbilitySystemComponent == nullptr)
		return;
	
	for (const auto Ability : DefaultActivateAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpecHandle	OutAbilitySpecHandle;
			URaidBossAbilityBase*		AbilityCDO = Cast<URaidBossAbilityBase>(Ability->GetDefaultObject());
			
			AbilitySystemComponent->GiveAbilityWithoutDuplication(Ability, OutAbilitySpecHandle, static_cast<int32>(AbilityCDO->GetAbilityInputID()));
			AbilitySystemComponent->TryActivateAbility(OutAbilitySpecHandle);
		}
	}
}

void ARaidBossCharacterBase::ApplyCharacterStatusEffect() const
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

		ContextHandle.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(CharacterStatusEffect.GetDefaultObject(), 1, ContextHandle);
	}
}

void ARaidBossCharacterBase::SetAnimationData(const UDataTable*	AnimDataTable)
{
	if (AnimDataTable == nullptr)
	{
		return;
	}
	
	for (int i = 1; i < static_cast<int>(EWeaponType::LastIndex); i++)
	{
		EWeaponType WeaponType = static_cast<EWeaponType>(i);
		FString RowName = UEnum::GetDisplayValueAsText(WeaponType).ToString();
		const FAnimationsByWeapon* DataRow = AnimDataTable->FindRow<FAnimationsByWeapon>(*RowName, FString());

		if (DataRow && DataRow->WeaponType == WeaponType)
		{
			AnimationsByWeaponType.Add(WeaponType, DataRow->AnimationsByWeaponType);
		}
	}
}

void ARaidBossCharacterBase::AddNewItemData(URaidBossItemBase* NewItem, FInventoryData& Data)
{
	FGameplayAbilitySpec	Spec { NewItem };
	
	Data.Item = NewItem;
	OwningItemSpecHandle.Add(NewItem->GetAbilityTriggerTag(), AbilitySystemComponent->GiveAbility(Spec));
	NotifyNewItemAdded.Broadcast(NewItem, Data.Amount);
}

void ARaidBossCharacterBase::GiveSkillAbilities()
{
	if (AbilitySystemComponent == nullptr)
		return;
	
	for (const auto PlayerSkill : PlayerSkills)
	{
		if (PlayerSkill)
		{
			FGameplayAbilitySpecHandle	AbilitySpecHandle;
			URaidBossAbilityBase*		AbilityCDO = Cast<URaidBossAbilityBase>(PlayerSkill->GetDefaultObject());
			
			AbilitySystemComponent->GiveAbilityWithoutDuplication(PlayerSkill, AbilitySpecHandle, static_cast<int32>(AbilityCDO->GetAbilityInputID()));
		}
	}
}

UAbilitySystemComponent* ARaidBossCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URaidBossAbilitySystemComponent* ARaidBossCharacterBase::GetRaidBossAbilitySystemComponent() const
{
	return Cast<URaidBossAbilitySystemComponent>(GetAbilitySystemComponent());
}

const URaidBossCharacterStatusAttributeSet* ARaidBossCharacterBase::GetCharacterStatusAttributeSet() const
{
	return CharacterStatusAttributeSet;
}

double ARaidBossCharacterBase::GetExperience() const
{
	return Experience;
}

double ARaidBossCharacterBase::GetMaxExperience() const
{
	return MaxExperience;
}

bool ARaidBossCharacterBase::IsCharacterStateTurnOn(ECharacterState CharacterState) const
{
	return CharacterStateBitMask & static_cast<int32>(CharacterState);
}

float ARaidBossCharacterBase::GetHealth() const
{
	return CharacterStatusAttributeSet->GetHealth();
}

float ARaidBossCharacterBase::GetAttackPower() const
{
	return CharacterStatusAttributeSet->GetAttackPower();
}

void ARaidBossCharacterBase::TurnOnCharacterStateBitMap(ECharacterState CharacterState)
{
	int32	Bit = static_cast<int32>(CharacterState);
	
	CharacterStateBitMask |= Bit;
}

void ARaidBossCharacterBase::TurnOffCharacterStateBitMap(ECharacterState CharacterState)
{
	int32	Bit = static_cast<int32>(CharacterState);
	
	CharacterStateBitMask &= ~Bit;
}

void ARaidBossCharacterBase::GiveExperience(double Exp)
{
	Experience += Exp;

	if (Experience >= MaxExperience)
	{
		CharacterLevelUp(floor(Experience / MaxExperience));
	}
}

void ARaidBossCharacterBase::CharacterLevelUp(float IncrementNum)
{
	Experience = fmod(Experience, MaxExperience);
	MaxExperience = MaxExperience * IncrementNum * 2;
	CharacterLevel += IncrementNum;
}

void ARaidBossCharacterBase::EquipWeapon(FWeaponKey WeaponKey)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponKey() != WeaponKey)
	{
		UnEquipWeapon();
	}
	if (!EquippedWeapon || EquippedWeapon->GetWeaponKey() != WeaponKey)
	{
		EquippedWeapon = Cast<AWeapon>(EquipManager->Equip(WeaponKey));
		
		if (EquippedWeapon)
		{
			const FWeaponData& WeaponData = EquippedWeapon->GetWeaponData();
	
			CurrentEquippedWeaponType = WeaponData.WeaponKey.WeaponType;
			PlayAnimMontage(WeaponData.WeaponAnimations.EquipAnim);
		}
	}
}

void ARaidBossCharacterBase::UnEquipWeapon()
{
	if (EquipManager && EquippedWeapon)
	{
		EquipManager->UnEquip(EquippedWeapon);
		CurrentEquippedWeaponType = EWeaponType::None;
		EquippedWeapon = nullptr;
	}
}

void ARaidBossCharacterBase::Attack() const
{
	if (EquippedWeapon && bCanActivateNormalAttack)
	{
		EquippedWeapon->Attack();
	}
}

void ARaidBossCharacterBase::SetCurrentCharacterState(EPlayerState State)
{
	switch (State)
	{
	case EPlayerState::Aiming :
		{
			if (GetCharacterMovement()->IsFalling())
			{
				return;
			}

			EquippedWeapon->AddWidgetToViewport();
			GetCharacterMovement()->MaxWalkSpeed = 150;
			break;
		}
	default:
		{
			EquippedWeapon->RemoveWidgetFromViewport();
			GetCharacterMovement()->MaxWalkSpeed = 600;
			break;
		}
	}
	
	CurrentPlayerState = State;
}

int32 ARaidBossCharacterBase::DecreaseOrRemoveInventoryData(FGameplayTag InAbilityTriggerTag, bool bRemoveAll/* = false*/,
	int32 Amount/* = 1*/)
{
	FInventoryData* Data = InventoryData.Find(InAbilityTriggerTag);
	
	if (Data == nullptr)
	{
		return 0;
	}
	
	Data->Amount -= Amount;

	int32 RemainingItems = Data->Amount;
	
	if (RemainingItems <= 0 || bRemoveAll)
	{
		if (IsItEquipment(Data->Item) == false)
		{
			RemoveAbilityData(InAbilityTriggerTag);
		}

		InventoryData.Remove(InAbilityTriggerTag);
	}
	
	NotifyItemAmountChanged.Broadcast(InAbilityTriggerTag, RemainingItems);

	return RemainingItems;
}

int32 ARaidBossCharacterBase::IncreaseOrAddInventoryData_ForBlueprint(TSubclassOf<URaidBossItemBase> NewItem,
	int32 Amount)
{
	URaidBossItemBase*	ItemCDO = NewItem ? NewItem->GetDefaultObject<URaidBossItemBase>() : nullptr;
	
	if (ItemCDO == nullptr || ItemCDO->GetAbilityTriggerTag().IsValid() == false || Amount <= 0)
	{
		return 0;
	}
	
	FInventoryData& Data = InventoryData.FindOrAdd(ItemCDO->GetAbilityTriggerTag());
	Data.Amount += Amount;
	
	if (OwningItemSpecHandle.Find(ItemCDO->GetAbilityTriggerTag()) == nullptr)
	{
		AddNewItemData(ItemCDO, Data);
	}
	else
	{
		NotifyItemAmountChanged.Broadcast(ItemCDO->GetAbilityTriggerTag(), Data.Amount);
	}

	return Data.Amount;
}

int32 ARaidBossCharacterBase::IncreaseOrAddInventoryData(URaidBossItemBase* NewItem, int32 Amount/* = 1*/)
{
	if (NewItem == nullptr || NewItem->GetAbilityTriggerTag().IsValid() == false || Amount <= 0)
	{
		return 0;
	}
	
	FInventoryData& Data = InventoryData.FindOrAdd(NewItem->GetAbilityTriggerTag());
	Data.Amount += Amount;
	
	if (OwningItemSpecHandle.Find(NewItem->GetAbilityTriggerTag()) == nullptr)
	{
		AddNewItemData(NewItem, Data);
	}
	else if (Data.Amount == 1)
	{
		Data.Item = NewItem;
		NotifyNewItemAdded.Broadcast(NewItem, Data.Amount);//여기서 밸류값 추가해야함
	}
	else
	{
		NotifyItemAmountChanged.Broadcast(NewItem->GetAbilityTriggerTag(), Data.Amount);
	}

	return Data.Amount;
}

void ARaidBossCharacterBase::RemoveAbilityData(FGameplayTag InAbilityTriggerTag)
{
	FGameplayAbilitySpecHandle	SpecHandle = OwningItemSpecHandle.FindRef(InAbilityTriggerTag);
	FGameplayAbilitySpec*	Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);

	if (Spec)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
		OwningItemSpecHandle.Remove(InAbilityTriggerTag);
	}
}
