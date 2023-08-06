#include "Abilities/Player/RaidBossSerathQSkill.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Kismet/GameplayStatics.h"

URaidBossSerathQSkill::URaidBossSerathQSkill()
{
	SkillInfo.SkillCost			= 0;
	SkillInfo.SkillLevel		= 0;
	SkillInfo.MaximumSkillLevel = 5;
	SkillInfo.MinimumSkillLevel = 0;

	FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	
	BlockAbilitiesWithTag.AddTag(BlockTag);
	AbilityTags.AddTag(BlockTag);
}

bool URaidBossSerathQSkill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void URaidBossSerathQSkill::EventReceivedCallback(const FGameplayEventData Payload)
{
	TArray<ARaidBossEnemyBase*> TargetArr = SelectTargets();
	
	ApplyEffectsToTargets(TargetArr);
}

void URaidBossSerathQSkill::EndAbilityCallback()
{
	WaitGameplayEvent->EndTask();
	ReleaseOwnerCharacterMovement();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URaidBossSerathQSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

TArray<ARaidBossEnemyBase*> URaidBossSerathQSkill::SelectTargets()
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

bool URaidBossSerathQSkill::ActivateTasks()
{
	PlayMontageAndWait	 = CreatePlayMontageAndWaitTask();
	FGameplayTag ThisTag = FGameplayTag::RequestGameplayTag(FName("Skill.Q"));
	WaitGameplayEvent	 = CreateWaitGameplayEventTask(ThisTag, false);
	
	if (::IsValid(PlayMontageAndWait) == false || ::IsValid(WaitGameplayEvent) == false)
		return false;

	PlayMontageAndWait->OnBlendOut.AddDynamic(this,	&URaidBossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->OnCancelled.AddDynamic(this,	&URaidBossSerathQSkill::EndAbilityCallback);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidBossSerathQSkill::EventReceivedCallback);
	WaitGameplayEvent->Activate();
	
	return true;
}

void URaidBossSerathQSkill::ApplyEffectsToTargets(TArray<ARaidBossEnemyBase*> TargetArr)
{
	FGameplayAbilityTargetData_ActorArray*	NewData = new FGameplayAbilityTargetData_ActorArray();
	
	for (const auto TargetObject : TargetArr)
	{
		if (TargetObject->IsCharacterStateTurnOn(ECharacterState::Alive) == false)
			continue;
		
		NewData->TargetActorArray.Add(TargetObject);
		
		FVector	Location = TargetObject->GetActorLocation();
		Location.Z = 0;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Location);
	}
	
	FGameplayAbilityTargetDataHandle	TargetData(NewData);
	FGameplayEffectSpecHandle			EffectSpecHandle = CreateEffectSpecHandle();
	TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetData);
}

float URaidBossSerathQSkill::CalculateAdditionalAttackPower()
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
