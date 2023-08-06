#include "Character/Player/RaidBossPlayerBase.h"
#include "Character/Player/RaidBossPlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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

	Tags.Add("Player");
}

void ARaidBossPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	ApplyCharacterStatusEffect();
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

ARaidBossPlayerControllerBase* ARaidBossPlayerBase::GetRaidBossPlayerController() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetController());
}

UCameraComponent* ARaidBossPlayerBase::GetFollowCamera() const
{
	return FollowCamera;
}
