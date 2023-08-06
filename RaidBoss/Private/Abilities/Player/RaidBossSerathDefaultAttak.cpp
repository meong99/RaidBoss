#include "Abilities/Player/RaidBossSerathDefaultAttak.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Abilities/Util/RaidBossComboSystem.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URaidBossSerathDefaultAttak::URaidBossSerathDefaultAttak()
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

void URaidBossSerathDefaultAttak::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

bool URaidBossSerathDefaultAttak::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void URaidBossSerathDefaultAttak::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if (IsActive() == true)
		bIsRetrrigered = true;
	else
		bIsRetrrigered = false;
}

void URaidBossSerathDefaultAttak::Tick(float DeltaTime)
{
	InterpolateAttackDirection(DeltaTime);
}

void URaidBossSerathDefaultAttak::EventReceivedCallback(const FGameplayEventData Payload)
{
	SetIsTickable(false);
	ApplyEffecsToTargets(SelectTargets());
}

void URaidBossSerathDefaultAttak::EndAbilityCallback()
{
	ResetAbility();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URaidBossSerathDefaultAttak::Interrupt()
{
	if (bIsRetrrigered == true)
	{
		ResetAbility();
	}
	else
	{
		EndAbilityCallback();
	}
}

bool URaidBossSerathDefaultAttak::ActivateTasks()
{
	PlayMontageAndWait	= CreatePlayMontageAndWaitTask(Combo->GetCurrentStack());
	FGameplayTag ThisTag		= FGameplayTag::RequestGameplayTag(FName("Attack.DefaultAttack"));
	WaitGameplayEvent	= CreateWaitGameplayEventTask(ThisTag);
	
	if (::IsValid(PlayMontageAndWait) == false || ::IsValid(WaitGameplayEvent) == false)
		return false;

	PlayMontageAndWait->OnBlendOut.AddDynamic(this,	&URaidBossSerathDefaultAttak::EndAbilityCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossSerathDefaultAttak::Interrupt);
	PlayMontageAndWait->OnCancelled.AddDynamic(this,	&URaidBossSerathDefaultAttak::EndAbilityCallback);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidBossSerathDefaultAttak::EventReceivedCallback);
	WaitGameplayEvent->Activate();
	return true;
}

void URaidBossSerathDefaultAttak::InterpolateAttackDirection(float DeltaTime)
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

TArray<ARaidBossEnemyBase*> URaidBossSerathDefaultAttak::SelectTargets()
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
			&& IsTargetInAngleXY(StandardVector, TargetVector, 90) == true)
		{
			TargetArr.Add(TergetObject);
		}
	}

	return TargetArr;
}

void URaidBossSerathDefaultAttak::ApplyEffecsToTargets(const TArray<ARaidBossEnemyBase*>& TargetArr)
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

void URaidBossSerathDefaultAttak::ResetAbility()
{
	SetIsTickable(false);
	ReleaseOwnerCharacterMovement();
	bIsRetrrigered = false;
}

float URaidBossSerathDefaultAttak::CalculateAdditialnalAttackPower()
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
