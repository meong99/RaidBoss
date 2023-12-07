#include "Character/RaidBossCharacterBase.h"
#include "Abilities/RaidBossAbilityBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/Item/RaidBossItemBase.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "Equipment/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/ItemData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Management/RaidBossGameplayTags.h"

ARaidBossCharacterBase::ARaidBossCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeLocation(FVector{0, 0, 200});

	AbilitySystemComponent = CreateDefaultSubobject<URaidBossAbilitySystemComponent>(TEXT("Ability Component"));
	CharacterStatusAttributeSet = CreateDefaultSubobject<URaidBossCharacterStatusAttributeSet>(TEXT("Character Status AttributeSet"));
	EquipManager = CreateDefaultSubobject<UEquipManagement>(TEXT("Equip Manager"));

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_ItemData.DT_ItemData'"));
	if (ItemDataTableObject.Object)
	{
		ItemDataTable = ItemDataTableObject.Object;
	}
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
}

void ARaidBossCharacterBase::PossessedBy(AController* NewController)
{
	ApplyCharacterDefaultSpecEffectToSelf();
	GiveDefaultAbilities();
	GiveAndActivateAbilities();
	GiveSkillAbilities();

	if (EquippedWeapon)
	{
		EquippedWeapon->NotifyNewWeaponEquipped();
	}
	
	Super::PossessedBy(NewController);
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
	InputMoveForward = Value.Y;
	InputMoveRight = Value.X;
	
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

void ARaidBossCharacterBase::ApplyCharacterDefaultSpecEffectToSelf()
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle OutGoingSpec = AbilitySystemComponent->MakeOutgoingSpec(CharacterStatusEffect, 1, ContextHandle);
		
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MaxHealth, CharacterDefaultSpec.MaxHealth);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MaxMana, CharacterDefaultSpec.MaxMana);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackPower, CharacterDefaultSpec.AttackPower);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackRange, CharacterDefaultSpec.AttackRange);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_DefensePower, CharacterDefaultSpec.DefencePower);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MoveSpeed, CharacterDefaultSpec.MoveSpeed);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackSpeed, CharacterDefaultSpec.AttackSpeed);

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*OutGoingSpec.Data);
	}
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

void ARaidBossCharacterBase::InitAnimationData(const UDataTable*	AnimDataTable)
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

void ARaidBossCharacterBase::AddNewItemData(URaidBossItemBase* NewItem, FInventoryData& Data, uint8 ItemKey)
{
	FGameplayAbilitySpecHandle	SpecHandle;
	
	Data.Item = NewItem;

	if (AbilitySystemComponent->GiveAbilityWithoutDuplication(NewItem->GetClass(), SpecHandle))
	{
		OwningItemSpecHandle.Add(NewItem->GetAbilityTriggerTag(), SpecHandle);
	}

	URaidBossItemBase*	NewItemInstance = Cast<URaidBossItemBase>(
		AbilitySystemComponent->GetInstanceAbilitiesByTag().FindRef(NewItem->GetAbilityTriggerTag()));

	if (NewItemInstance)
	{
		NewItemInstance->SetItemKey(ItemKey);
	}
	
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
			RemoveItemAbility(InAbilityTriggerTag);
		}

		InventoryData.Remove(InAbilityTriggerTag);
	}
	
	NotifyItemAmountChanged.Broadcast(InAbilityTriggerTag, RemainingItems);

	return RemainingItems;
}

int32 ARaidBossCharacterBase::IncreaseOrAddInventoryData(uint8 ItemKey, int32 Amount)
{
	if (ItemDataTable == nullptr)
	{
		return 0;
	}

	FItemDataTable*		ItemData = ItemDataTable->FindRow<FItemDataTable>(FName(FString::FromInt(ItemKey)), "");
	URaidBossItemBase*	ItemCDO = ItemData ? ItemData->ItemClass->GetDefaultObject<URaidBossItemBase>() : nullptr;

	if (ItemCDO == nullptr || ItemCDO->GetAbilityTriggerTag().IsValid() == false)
	{
		return 0;
	}
	
	FInventoryData& Data = InventoryData.FindOrAdd(ItemCDO->GetAbilityTriggerTag());
	Data.Amount += Amount;
	
	if (OwningItemSpecHandle.Find(ItemCDO->GetAbilityTriggerTag()) == nullptr)
	{
		AddNewItemData(ItemCDO, Data, ItemKey);
	}
	else if (Data.Amount == 1)
	{
		Data.Item = ItemCDO;
		NotifyNewItemAdded.Broadcast(ItemCDO, Data.Amount);
	}
	else
	{
		NotifyItemAmountChanged.Broadcast(ItemCDO->GetAbilityTriggerTag(), Data.Amount);
	}

	return Data.Amount;
}
// IncreaseOrAddInventoryData(uint8 ItemKey, int32 Amount)메소드 확인 후 제거 예정
// int32 ARaidBossCharacterBase::IncreaseOrAddInventoryData(URaidBossItemBase* NewItem, int32 Amount/* = 1*/)
// {
// 	if (NewItem == nullptr || NewItem->GetAbilityTriggerTag().IsValid() == false || Amount <= 0)
// 	{
// 		return 0;
// 	}
// 	
// 	FInventoryData& Data = InventoryData.FindOrAdd(NewItem->GetAbilityTriggerTag());
// 	Data.Amount += Amount;
// 	
// 	if (OwningItemSpecHandle.Find(NewItem->GetAbilityTriggerTag()) == nullptr)
// 	{
// 		AddNewItemData(NewItem, Data);
// 	}
// 	else if (Data.Amount == 1)
// 	{
// 		Data.Item = NewItem;
// 		NotifyNewItemAdded.Broadcast(NewItem, Data.Amount);
// 	}
// 	else
// 	{
// 		NotifyItemAmountChanged.Broadcast(NewItem->GetAbilityTriggerTag(), Data.Amount);
// 	}
//
// 	return Data.Amount;
// }

void ARaidBossCharacterBase::RemoveItemAbility(FGameplayTag InAbilityTriggerTag)
{
	FGameplayAbilitySpecHandle	SpecHandle = OwningItemSpecHandle.FindRef(InAbilityTriggerTag);
	FGameplayAbilitySpec*	Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);

	if (Spec)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
		OwningItemSpecHandle.Remove(InAbilityTriggerTag);
	}
}
