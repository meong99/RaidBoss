#include "Abilities/Player/RaidbossSerathQSkill.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Kismet/GameplayStatics.h"

URaidbossSerathQSkill::URaidbossSerathQSkill()
{
	SkillInfo.SkillCost			= 0;
	SkillInfo.SkillLevel		= 0;
	SkillInfo.MaximumSkillLevel = 5;
	SkillInfo.MinimumSkillLevel = 0;

	FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	BlockAbilitiesWithTag.AddTag(BlockTag);
	AbilityTags.AddTag(BlockTag);
}

void URaidbossSerathQSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		EndAbilityCallback();
	}
	else if (ActivateTasks() == false)
	{
		EndAbilityCallback();
	}
	else
	{
		BlockOwnerCharacterMovement();
	}
}

bool URaidbossSerathQSkill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void URaidbossSerathQSkill::EventReceivedCallback(const FGameplayEventData Payload)
{
	TArray<ARaidBossEnemyBase*> TargetArr = SelectTargets();
	ApplyEffecsToTargets(TargetArr);
}

void URaidbossSerathQSkill::EndAbilityCallback()
{
	WaitGameplayEvent->EndTask();
	ReleaseOwnerCharacterMovement();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool URaidbossSerathQSkill::ActivateTasks()
{
	PlayMontageAndWait	= CreatePlayMontageAndWaitTask();
	FGameplayTag ThisTag		= FGameplayTag::RequestGameplayTag(FName("Skill.Q"));
	WaitGameplayEvent	= CreateWaitGameplayEventTask(ThisTag, false);
	
	if (::IsValid(PlayMontageAndWait) == false || ::IsValid(WaitGameplayEvent) == false)
		return false;

	PlayMontageAndWait->OnBlendOut.AddDynamic(this,	&URaidbossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidbossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->OnCancelled.AddDynamic(this,	&URaidbossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidbossSerathQSkill::EventReceivedCallback);
	WaitGameplayEvent->Activate();
	return true;
}

TArray<ARaidBossEnemyBase*> URaidbossSerathQSkill::SelectTargets()
{
	TArray<AActor*>	OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaidBossEnemyBase::StaticClass(), OutArray);
	TArray<ARaidBossEnemyBase*> TargetArr;
	
	if (IsValid(OwnerCharacter) == false)
		return TargetArr;
	
	for (const auto Element : OutArray)
	{
		ARaidBossEnemyBase* EnermyObject = Cast<ARaidBossEnemyBase>(Element);

		if (IsValid(EnermyObject) && IsTargetInRangeXY(EnermyObject, Range) == true)
		{
			TargetArr.Add(EnermyObject);
		}
	}

	return TargetArr;
}

void URaidbossSerathQSkill::ApplyEffecsToTargets(TArray<ARaidBossEnemyBase*> TargetArr)
{
	for (const auto& TergetObject : TargetArr)
	{
		FGameplayAbilityTargetDataHandle	TargetData = CreateAbilityTargetDataFromActor(TergetObject);
		float Magnitude					 = CalculateAdditialnalAttackPower();
        FGameplayTag DataTag		 	 = FGameplayTag::RequestGameplayTag(FName("DamageExecution"));
        FGameplayEffectSpecHandle Handle = SetCallerMagnitudeByDataTag(EffectClass, DataTag, Magnitude, SkillInfo.SkillLevel);
		
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Handle, TargetData);
		
		FVector	Location = TergetObject->GetActorLocation();
		Location.Z = 0;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Location);
	}
}

float URaidbossSerathQSkill::CalculateAdditialnalAttackPower()
{
	int		SkillLevel			= SkillInfo.SkillLevel;
	float	TotalIncreaseRate	= (SkillLevel - 1) * IncreaseRate;
	float	AdditialnalAttackPower = 0;
	
	
	if (IsValid(OwnerCharacter) == true)
	{
		AdditialnalAttackPower = OwnerCharacter->GetAttackPower() * (TotalIncreaseRate / 100);
	}

	return AdditialnalAttackPower;
}
