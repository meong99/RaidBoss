#include "Abilities/Enermy/RaidBossEnemyDefaultAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Kismet/GameplayStatics.h"

URaidBossEnemyDefaultAttack::URaidBossEnemyDefaultAttack()
{
	SkillInfo.SkillCost = 0;
	SkillInfo.SkillLevel = 1;
	SkillInfo.MaximumSkillLevel = 10;
	SkillInfo.MinimumSkillLevel = 1;

	FGameplayTag	BlockTag = FGameplayTag::RequestGameplayTag(FName("Block.OtherSkill"));
	BlockAbilitiesWithTag.AddTag(BlockTag);
	AbilityTags.AddTag(BlockTag);
}

void URaidBossEnemyDefaultAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &URaidBossEnemyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &URaidBossEnemyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &URaidBossEnemyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &URaidBossEnemyDefaultAttack::EndAbilityCallback);
		PlayMontageAndWait->Activate();
		
		WaitGameplayEvent->EventReceived.AddDynamic(this, &URaidBossEnemyDefaultAttack::EventReceivedCallback);
		WaitGameplayEvent->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Ability can not activate"));
		EndAbilityCallback();
	}
}

void URaidBossEnemyDefaultAttack::EventReceivedCallback(const FGameplayEventData Payload)
{
	ApplyEffectsToTargets(SelectTargets());
}

void URaidBossEnemyDefaultAttack::EndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

TArray<ARaidBossPlayerBase*> URaidBossEnemyDefaultAttack::SelectTargets()
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
			&& IsTargetInAngleXY(StandardVector, TargetVector, 90) == true
			&& TargetObject->IsCharacterStateTurnOn(ECharacterState::Alive))
		{
			TargetArr.Add(TargetObject);
		}
	}

	return TargetArr;
}

void URaidBossEnemyDefaultAttack::ApplyEffectsToTargets(const TArray<ARaidBossPlayerBase*>& TargetArr)
{
	FGameplayAbilityTargetData_ActorArray*	NewData = new FGameplayAbilityTargetData_ActorArray();

	for (const auto TargetObject : TargetArr)
	{
		NewData->TargetActorArray.Add(TargetObject);

		FGameplayAbilityTargetDataHandle	TargetData = CreateAbilityTargetDataFromActor(TargetObject);
	}
		
	FGameplayAbilityTargetDataHandle	TargetData(NewData);
	FGameplayEffectSpecHandle			EffectSpecHandle = CreateEffectSpecHandle();
	TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetData);
}
