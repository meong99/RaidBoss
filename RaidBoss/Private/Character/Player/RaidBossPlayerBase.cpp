#include "Character/Player/RaidBossPlayerBase.h"

#include "Abilities/RaidBossAbilitySystemComponent.h"
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
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -98.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeLocation(FVector{0, 0, 100});

	Tags.Add("Player");
}

void ARaidBossPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	const TCHAR* AnimDataLink =
		TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_Player_AnimationByWeaponType.DT_Player_AnimationByWeaponType'");
	const UDataTable*	AnimDataTable = LoadObject<UDataTable>(nullptr, AnimDataLink);

	InitAnimationData(AnimDataTable);
}

void ARaidBossPlayerBase::JumpCharacter()
{
	Super::Jump();
}

void ARaidBossPlayerBase::StopJumpCharacter()
{
	Super::StopJumping();
}

ARaidBossPlayerControllerBase* ARaidBossPlayerBase::GetRaidBossPlayerController() const
{
	return Cast<ARaidBossPlayerControllerBase>(GetController());
}
