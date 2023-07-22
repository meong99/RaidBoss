#include "Character/Player/RaidBossPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Math/UnrealMathUtility.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Global/RaidBossInteractionBase.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/Item/RaidBossEquipmentItem.h"

ARaidBossPlayerBase::ARaidBossPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Tags.Add(PLAYER_TAG);
}

void ARaidBossPlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// CreateSkillObjects();
	ApplyCharacterStatusEffect();
}

void ARaidBossPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddMappingSystem();
	BindInputAction(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}

void ARaidBossPlayerBase::AddMappingSystem()
{
	
	const auto	PlayerController = GetRiadBossPlayerController();
	if (IsValid(PlayerController) == true)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(SubSystem) == true)
		{
			SubSystem->AddMappingContext(PlayerController->GetInputAction().DefaultContext, 0);
		}
	}
}

void ARaidBossPlayerBase::BindInputAction(UEnhancedInputComponent* EnhancedInputComponent)
{
	const auto	PlayerController = GetRiadBossPlayerController();
	if (IsValid(PlayerController) == false || IsValid(EnhancedInputComponent) == false)
		return;
	
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().MoveAction,
		ETriggerEvent::Triggered,	this, &ARaidBossPlayerBase::MoveCharacter);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().LookAction,
		ETriggerEvent::Triggered,	this, &ARaidBossPlayerBase::LookCharacter);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().JumpAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::Jump);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().JumpAction,
		ETriggerEvent::Completed,	this, &ARaidBossPlayerBase::StopJumping);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().InteractionAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::Interaction);
	
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().DefaultAttackAction,
		ETriggerEvent::Triggered,	this, &ARaidBossPlayerBase::ActiveAbilityByInput, ERaidBossAbilityInputID::DefaultAttack);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().QAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::ActiveAbilityByInput, ERaidBossAbilityInputID::Q);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().EAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::ActiveAbilityByInput, ERaidBossAbilityInputID::E);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().RAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::ActiveAbilityByInput, ERaidBossAbilityInputID::R);
	EnhancedInputComponent->BindAction(PlayerController->GetInputAction().DashAction,
		ETriggerEvent::Started,		this, &ARaidBossPlayerBase::ActiveAbilityByInput, ERaidBossAbilityInputID::Dash);
}

//
// void ARaidBossPlayerBase::CreateSkillObjects()
// {
// 	ARaidBossPlayerControllerBase*			PlayerController;
// 	TArray<TSubclassOf<URaidBossSkillBase>> SkillClasses;
// 	URaidBossSkillBase*						SkillCDO;
// 	URaidBossSkillBase*						SkillObject;
// 	FGameplayAbilitySpecHandle				SpecHandle;
// 	
// 	PlayerController = GetRiadBossPlayerController();
// 	if (PlayerController && AbilitySystemComponent)
// 	{
// 		for (auto SkillClass : SkillClasses)
// 		{
// 			SkillCDO = Cast<URaidBossSkillBase>(SkillClass->GetDefaultObject());
// 			AbilitySystemComponent->GiveAbilityWithoutDuplication(SkillClass, SpecHandle, static_cast<int32>(SkillCDO->AbilityInputID));
//
// 			SkillObject = Cast<URaidBossSkillBase>(AbilitySystemComponent->GetAbilityByClass(SkillClass));
// 			if (SkillObject) SkillObjects.Add(SkillObject);
// 		}
// 	}
// }
//
// void ARaidBossPlayerBase::CreateItemObjects()
// {
// 	ARaidBossPlayerControllerBase*			PlayerController;
// 	TArray<TSubclassOf<URaidBossItemBase>>	ItemClasses;
// 	URaidBossUserWidget*					ItemWidget;
// 	URaidBossItemBase*						ItemObject;
// 	FGameplayAbilitySpecHandle				SpecHandle;
// 	
// 	PlayerController = GetRiadBossPlayerController();
// 	if (PlayerController && AbilitySystemComponent)
// 	{
// 		ItemClasses = PlayerController->GetItemClasses();
// 		for (const auto& ItemClass : ItemClasses)
// 		{
// 			AbilitySystemComponent->GiveAbilityWithoutDuplication(ItemClass, SpecHandle);
// 			
// 			ItemObject = Cast<URaidBossItemBase>(AbilitySystemComponent->GetAbilityByClass(ItemClass));
// 			if (ItemObject) ItemObjects.Add(ItemObject);
//
// 			ItemWidget = PlayerController->GetInventoryWidget();
// 			if (ItemWidget) ItemWidget->AddAbility(ItemObject);
// 		}
// 	}
// }

void ARaidBossPlayerBase::ApplyCharacterStatusEffect()
{
	ARaidBossPlayerControllerBase*	PlayerController;

	PlayerController = GetRiadBossPlayerController();
	if (PlayerController && AbilitySystemComponent)
	{
		TSubclassOf<UGameplayEffect> EffectClass = PlayerController->GetCharacterStatusEffect();
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

		ContextHandle.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectClass.GetDefaultObject(), 1, ContextHandle);
	}
}

void ARaidBossPlayerBase::Jump()
{
	if (IsCharacterStateTurnOn(ECharacterState::CanMove))
	{
		Super::Jump();
	}
}

void ARaidBossPlayerBase::StopJumping()
{
	Super::StopJumping();
}

void ARaidBossPlayerBase::MoveCharacter(const FInputActionValue& Value)
{
	if (IsCharacterStateTurnOn(ECharacterState::CanMove))
	{
		FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		FVector Right = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

		AddMovementInput(Forward, Value.Get<FVector2D>().Y);
		AddMovementInput(Right, Value.Get<FVector2D>().X);
	}
}

void ARaidBossPlayerBase::LookCharacter(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<FVector2D>().X * 0.4);
	AddControllerPitchInput(Value.Get<FVector2D>().Y * 0.4);
}

void ARaidBossPlayerBase::Interaction(const FInputActionValue& Value)
{
	for (const auto& Actor : InteractionableActors)
	{
		Actor->OnInteration(this);
	}
}

void ARaidBossPlayerBase::ActiveAbilityByInput(const FInputActionValue& Value, ERaidBossAbilityInputID InputID)
{
	if (IsValid(AbilitySystemComponent) == false)
		return;
	
	URaidBossAbilityBase* Ability = AbilitySystemComponent->GetAbilityByInputID(static_cast<int32>(InputID));
	
	if (Ability && Value.Get<bool>() == true)
	{
		AbilitySystemComponent->TryActivateAbility(Ability->GetCurrentAbilitySpecHandle());
	}
}

TArray<ARaidBossInteractionBase*> ARaidBossPlayerBase::GetInteractionableActors() const
{
	return InteractionableActors;
}

float ARaidBossPlayerBase::GetGold() const
{
	return Gold;
}

ARaidBossPlayerControllerBase* ARaidBossPlayerBase::GetRiadBossPlayerController() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetController());
}
