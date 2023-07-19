#include "Abilities/Enermy/RaidBossEnermyDefaultAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Kismet/GameplayStatics.h"

URaidBossEnermyDefaultAttack::URaidBossEnermyDefaultAttack()
{
	SkillInfo.SkillCost = 0;
	SkillInfo.SkillLevel = 1;
	SkillInfo.MaximumSkillLevel = 10;
	SkillInfo.MinimumSkillLevel = 1;

	FGameplayTag	BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	BlockAbilitiesWithTag.AddTag(BlockTag);
	AbilityTags.AddTag(BlockTag);
}

void URaidBossEnermyDefaultAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability can not activate"));
		EndAbilityCallback();
		return;
	}

	PlayMontageAndWait = CreatePlayMontageAndWaitTask();
	FGameplayTag	ThisTag = FGameplayTag::RequestGameplayTag(FName("Attack.DefaultAttack"));
	WaitGameplayEvent = CreateWaitGameplayEventTask(ThisTag);
	
	if (::IsValid(PlayMontageAndWait) == true && ::IsValid(WaitGameplayEvent) == true)
	{
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &URaidBossEnermyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &URaidBossEnermyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossEnermyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &URaidBossEnermyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->Activate();
		
		WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidBossEnermyDefaultAttack::EventReceivedCallback);
		WaitGameplayEvent->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Ability can not activate"));
		EndAbilityCallback();
	}
}

void URaidBossEnermyDefaultAttack::EventReceivedCallback(const FGameplayEventData Payload)
{
	ApplyEffecsToTargets(SelectTargets());
}

void URaidBossEnermyDefaultAttack::EndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

TArray<ARaidBossPlayerBase*> URaidBossEnermyDefaultAttack::SelectTargets()
{
	TArray<AActor*>	OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaidBossPlayerBase::StaticClass(), OutArray);
	TArray<TObjectPtr<ARaidBossPlayerBase>>	TargetArr;
	
	if (IsValid(OwnerCharacter) == false)
		return TargetArr;
	
	for (const auto Element : OutArray)
	{
		ARaidBossPlayerBase* TargetObject	= Cast<ARaidBossPlayerBase>(Element);
		float	EnermyCapsuleRadius			= TargetObject->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float	CharacterRange				= OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackRange();
		FVector	StandardVector				= OwnerCharacter->GetActorForwardVector();
		FVector	TargetVector				= TargetObject->GetActorLocation() - OwnerCharacter->GetActorLocation();

		if (IsTargetInRangeXY(TargetObject, CharacterRange + EnermyCapsuleRadius) == true 
			&& IsTargetInAngleXY(StandardVector, TargetVector, 90) == true)
		{
			TargetArr.Add(TargetObject);
		}
	}

	return TargetArr;
}

void URaidBossEnermyDefaultAttack::ApplyEffecsToTargets(const TArray<ARaidBossPlayerBase*>& TargetArr)
{
	for (const auto TargetObject : TargetArr)
	{
		FGameplayAbilityTargetDataHandle	TargetData = CreateAbilityTargetDataFromActor(TargetObject);
		
		for (const auto Effect : Effects)
		{
			ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, Effect, SkillInfo.SkillLevel);
		}
	}
}
