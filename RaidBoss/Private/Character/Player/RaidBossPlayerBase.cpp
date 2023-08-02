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
	
	ApplyCharacterStatusEffect();
}

void ARaidBossPlayerBase::ApplyCharacterStatusEffect()
{
	ARaidBossPlayerControllerBase*	PlayerController;

	PlayerController = GetRaidBossPlayerController();
	if (PlayerController && AbilitySystemComponent)
	{
		TSubclassOf<UGameplayEffect> EffectClass = PlayerController->GetCharacterStatusEffect();
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

		ContextHandle.AddSourceObject(this);
		AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectClass.GetDefaultObject(), 1, ContextHandle);
	}
}

void ARaidBossPlayerBase::JumpCharacter()
{
	if (IsCharacterStateTurnOn(ECharacterState::CanMove))
	{
		Super::Jump();
	}
}

void ARaidBossPlayerBase::StopJumpCharacter()
{
	Super::StopJumping();
}

void ARaidBossPlayerBase::MoveCharacter(const FVector2D& Value)
{
	if (IsCharacterStateTurnOn(ECharacterState::CanMove))
	{
		FVector Forward = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		FVector Right = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

		AddMovementInput(Forward, Value.Y);
		AddMovementInput(Right, Value.X);
	}
}

void ARaidBossPlayerBase::LookCharacter(const FVector2D& Value)
{
	AddControllerYawInput(Value.X * 0.4);
	AddControllerPitchInput(Value.Y * 0.4);
}

void ARaidBossPlayerBase::Interaction()
{
	for (const auto& Actor : InteractionableActors)
	{
		Actor->OnInteration(this);
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

ARaidBossPlayerControllerBase* ARaidBossPlayerBase::GetRaidBossPlayerController() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetController());
}
