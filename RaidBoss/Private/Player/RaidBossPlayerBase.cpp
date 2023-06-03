#include "Player/RaidBossPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

void ARaidBossPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARaidBossPlayerBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARaidBossPlayerBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm", "Cancel", FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("ERaidBossAbilityInputID"))),
			static_cast<int32>(ERaidBossAbilityInputID::Confirm),
			static_cast<int32>(ERaidBossAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ARaidBossPlayerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm", "Cancel", FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("ERaidBossAbilityInputID"))),
			static_cast<int32>(ERaidBossAbilityInputID::Confirm),
			static_cast<int32>(ERaidBossAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ARaidBossPlayerBase::MoveForward(float value)
{
	FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

	AddMovementInput(Direction, value);
}

void ARaidBossPlayerBase::MoveRight(float value)
{
	FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

	AddMovementInput(Direction, value);
}

USpringArmComponent* ARaidBossPlayerBase::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* ARaidBossPlayerBase::GetFollowCamera() const
{
	return FollowCamera;
}