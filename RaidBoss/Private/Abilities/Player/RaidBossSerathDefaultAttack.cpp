#include "Abilities/Player/RaidBossSerathDefaultAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/Util/RaidBossComboSystem.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URaidBossSerathDefaultAttack::URaidBossSerathDefaultAttack()
{
	SkillInfo.SkillCost			= 0;
	SkillInfo.SkillLevel		= 1;
	SkillInfo.MaximumSkillLevel = 10;
	SkillInfo.MinimumSkillLevel = 1;

	FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	BlockAbilitiesWithTag.AddTag(BlockTag);
	AbilityTags.AddTag(BlockTag);

	Combo = CreateDefaultSubobject<URaidBossComboSystem>("Combo System");
	Combo->SetMaxStack(4);
	Combo->SetResetTime(1.5);
}

bool URaidBossSerathDefaultAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool CanActivate			= Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) &&
								SkillInfo.SkillLevel > 0;
	bool OwnerCharacterState	= IsValid(OwnerCharacter) &&
								OwnerCharacter->IsCharacterStateTurnOn(ECharacterState::CanMove) &&
								OwnerCharacter->IsCharacterStateTurnOn(ECharacterState::CanUsingAttack) &&
								OwnerCharacter->GetCharacterMovement()->IsFalling() == false;
	
	bool Result = CanActivate && OwnerCharacterState;
	
	return Result;
}

void URaidBossSerathDefaultAttack::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (IsActive() == true)
		bIsRetriggered = true;
	else
		bIsRetriggered = false;
}

void URaidBossSerathDefaultAttack::Tick(float DeltaTime)
{
	InterpolateAttackDirection(DeltaTime);
}

void URaidBossSerathDefaultAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
												  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
												  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability can't activating not yet"));
		EndAbilityCallback();
	}
	else if (ActivateTasks() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability Tasks not created"));
		EndAbilityCallback();
	}
	else
	{
		Combo->SetIsTickable(true);
		Combo->IncreaseStack();
		SetIsTickable(true);
		BlockOwnerCharacterMovement();
	}
}

void URaidBossSerathDefaultAttack::EventReceivedCallback(const FGameplayEventData Payload)
{
	SetIsTickable(false);
	ApplyEffectsToTargets(SelectTargets());
}

void URaidBossSerathDefaultAttack::EndAbilityCallback()
{
	ResetAbility();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URaidBossSerathDefaultAttack::Interrupt()
{
	if (bIsRetriggered == true)
	{
		ResetAbility();
	}
	else
	{
		EndAbilityCallback();
	}
}

bool URaidBossSerathDefaultAttack::ActivateTasks()
{
	PlayMontageAndWait	 = CreatePlayMontageAndWaitTask(Combo->GetCurrentStack());
	FGameplayTag ThisTag = FGameplayTag::RequestGameplayTag(FName("Attack.DefaultAttack"));
	WaitGameplayEvent	 = CreateWaitGameplayEventTask(ThisTag);
	
	if (::IsValid(PlayMontageAndWait) == false || ::IsValid(WaitGameplayEvent) == false)
		return false;

	PlayMontageAndWait->OnBlendOut.AddDynamic(this,	&URaidBossSerathDefaultAttack::EndAbilityCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossSerathDefaultAttack::Interrupt);
	PlayMontageAndWait->OnCancelled.AddDynamic(this,	&URaidBossSerathDefaultAttack::EndAbilityCallback);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidBossSerathDefaultAttack::EventReceivedCallback);
	WaitGameplayEvent->Activate();
	return true;
}

void URaidBossSerathDefaultAttack::InterpolateAttackDirection(float DeltaTime)
{
	ARaidBossPlayerBase* Onwer = Cast<ARaidBossPlayerBase>(OwnerCharacter);
	
	if (IsValid(Onwer) == true)
	{
		UCameraComponent* FollowCamera = Onwer->GetFollowCamera();
		if (FollowCamera == nullptr)
			return;
		
		FRotator OwnerRotation	= Onwer->GetActorRotation();
		FRotator CameraRotator	= FollowCamera->GetForwardVector().Rotation();
		CameraRotator.Pitch = 0;
		CameraRotator.Roll = 0;
		
		Onwer->SetActorRotation(FMath::RInterpTo(OwnerRotation, CameraRotator, DeltaTime, 10.f));
	}
}

TArray<ARaidBossEnemyBase*> URaidBossSerathDefaultAttack::SelectTargets()
{
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaidBossEnemyBase::StaticClass(), OutArray);
	TArray<ARaidBossEnemyBase*> TargetArr;

	if (IsValid(OwnerCharacter) == false)
		return TargetArr;
	
	for (const auto Element : OutArray)
	{
		ARaidBossEnemyBase* TergetObject	= Cast<ARaidBossEnemyBase>(Element);
		float	EnermyCapsuleRadius 		= TergetObject->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float	CharacterRange				= OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackRange();
		FVector	StandardVector				= OwnerCharacter->GetActorForwardVector();
		FVector	TargetVector				= TergetObject->GetActorLocation() - OwnerCharacter->GetActorLocation();

		if (IsTargetInRangeXY(TergetObject, CharacterRange + EnermyCapsuleRadius) == true 
			&& IsTargetInAngleXY(StandardVector, TargetVector, 90) == true
			&& TergetObject->IsCharacterStateTurnOn(ECharacterState::Alive))
		{
			TargetArr.Add(TergetObject);
		}
	}

	return TargetArr;
}

void URaidBossSerathDefaultAttack::ApplyEffectsToTargets(const TArray<ARaidBossEnemyBase*>& TargetArr)
{
	FGameplayAbilityTargetData_ActorArray*	NewData = new FGameplayAbilityTargetData_ActorArray();
	
	for (const auto TargetObject : TargetArr)
	{
		NewData->TargetActorArray.Add(TargetObject);
	}
	
	FGameplayAbilityTargetDataHandle	TargetData(NewData);
	FGameplayEffectSpecHandle			EffectSpecHandle = CreateEffectSpecHandle();
	TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetData);
}

void URaidBossSerathDefaultAttack::ResetAbility()
{
	SetIsTickable(false);
	ReleaseOwnerCharacterMovement();
	bIsRetriggered = false;
}

float URaidBossSerathDefaultAttack::CalculateAdditionalAttackPower()
{
	int		SkillLevel			= SkillInfo.SkillLevel;
	float	TotalIncreaseRate	= (SkillLevel - 1) * IncreaseRate;
	float	AdditialnalAttackPower = 0;
	
	const URaidBossCharacterStatusAttributeSet* CharacterState = GetOwnerCharacterState();
	if (IsValid(CharacterState) == true)
	{
		AdditialnalAttackPower = CharacterState->GetAttackPower() * (TotalIncreaseRate / 100);
	}

	return AdditialnalAttackPower;
}
