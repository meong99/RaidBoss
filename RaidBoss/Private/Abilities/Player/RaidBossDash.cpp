#include "Abilities/Player/RaidBossDash.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Util/RaidBossLibrary.h"

URaidBossDash::URaidBossDash()
{
	SkillInfo.SkillCost			= 0;
	SkillInfo.SkillLevel		= 1;
	SkillInfo.MaximumSkillLevel = 1;
	SkillInfo.MinimumSkillLevel = 1;

	FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	BlockAbilitiesWithTag.AddTag(BlockTag);
}

void URaidBossDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Ability can't activating not yet"));
		EndAbilityCallback();
	}
	else if (ActivateTasks() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Ability Tasks not created"));
		EndAbilityCallback();
	}
	else
	{
		BlockOwnerCharacterMovement();
		DashDestination = GetDestination();
		OwnerCharacter->SetActorRotation(FRotator(0.f, (DashDestination - OwnerCharacter->GetActorLocation()).Rotation().Yaw, 0.f));
		SetIsTickable(true);
	}
}

bool URaidBossDash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate			= Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) &&
								  (SkillInfo.SkillLevel > 0);
	bool bOwnerCharacterState	= IsValid(OwnerCharacter) &&
								  OwnerCharacter->GetCharacterMovement()->IsFalling() == false;
	
	bool bResult = bCanActivate && bOwnerCharacterState;
	
	return bResult;
}

void URaidBossDash::Tick(float DeltaTime)
{
	if (IsValid(OwnerCharacter) == true)
	{
		FVector	Start;
		FVector End;

		Start = OwnerCharacter->GetActorLocation();
		End = DashDestination;
		OwnerCharacter->SetActorLocation(FMath::VInterpTo(Start, InterpolateDestinationByTracing(Start, End), DeltaTime, 7.5f));
		
		if (OwnerCharacter->GetActorLocation() == DashDestination)
			EndAbilityCallback();
	}
}

void URaidBossDash::EndAbilityCallback()
{
	SetIsTickable(false);
	ReleaseOwnerCharacterMovement();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool URaidBossDash::ActivateTasks()
{
	PlayMontageAndWait	= CreatePlayMontageAndWaitTask();
	
	if (::IsValid(PlayMontageAndWait) == false || IsValid(OwnerCharacter) == false)
		return false;

	PlayMontageAndWait->OnCompleted.AddDynamic(this,	&URaidBossDash::EndAbilityCallback);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this,	&URaidBossDash::EndAbilityCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossDash::EndAbilityCallback);
	PlayMontageAndWait->OnCancelled.AddDynamic(this,	&URaidBossDash::EndAbilityCallback);
	PlayMontageAndWait->Activate();

	return true;
}

FVector URaidBossDash::GetDestination()
{
	FVector					Direction;
	APlayerController*		Controller;
	ARaidBossPlayerBase*	Player;

	Controller	= Cast<APlayerController>(OwnerCharacter->GetController());
	Player		= Cast<ARaidBossPlayerBase>(OwnerCharacter);
	if (IsValid(Controller) == true && IsValid(Player))
	{
		FVector	CameraVector;
		UCameraComponent* FollowCamera = Player->GetFollowCamera();

		if (Controller->IsInputKeyDown(EKeys::A) == true)
		{
			CameraVector = FollowCamera->GetRightVector();
			CameraVector.Z = 0;
			CameraVector.Normalize();
			Direction += (CameraVector * DashRange * -1);
		}
		else if (Controller->IsInputKeyDown(EKeys::D) == true)
		{
			CameraVector = FollowCamera->GetRightVector();
			CameraVector.Z = 0;
			CameraVector.Normalize();
			Direction += (CameraVector * DashRange);
		}
		if (Controller->IsInputKeyDown(EKeys::S) == true)
		{
			CameraVector = FollowCamera->GetForwardVector();
			CameraVector.Z = 0;
			CameraVector.Normalize();
			Direction += (CameraVector * DashRange * -1);
		}
		else if (Controller->IsInputKeyDown(EKeys::W) == true || Direction.Length() < 1)
		{
			CameraVector = FollowCamera->GetForwardVector();
			CameraVector.Z = 0;
			CameraVector.Normalize();
			Direction += (CameraVector * DashRange);
		}
	}

	return OwnerCharacter->GetActorLocation() + Direction;
}

FVector URaidBossDash::InterpolateDestinationByTracing(FVector Start, FVector End)
{
	FHitResult	Result;
	float		CapsuleRadius;

	CapsuleRadius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	if (RaidBossLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, CapsuleRadius, Result) == true)
	{
		FVector		NewStart;
		
		NewStart = Result.Location + Result.Normal * CapsuleRadius;
		if (RaidBossLibrary::SphereTraceSingleForObjects(GetWorld(), NewStart, End, CapsuleRadius, Result) == true)
		{
			return Result.Location;
		}
	}

	return End;
}
