// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Sevarog/Ability_Sevarog_Q.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Abilities/Skill/SkillIndicator.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

UAbility_Sevarog_Q::UAbility_Sevarog_Q()
{
	SkillDamageRate = 2.f;
}

void UAbility_Sevarog_Q::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEvent*	WaitGameplayEvent = CreateWaitGameplayEventTask(RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, false);

	if (bIsFirstTrigger && Cast<AMonsterBase>(OwnerCharacter) == nullptr)
	{
		bIsFirstTrigger = false;
		SetIndicator();
	}
	else
	{
		bIsFirstTrigger = true;
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait;
		if (Cast<AMonsterBase>(OwnerCharacter))
		{
			PlayMontageAndWait = CreatePlayMontageAndWaitTask();
			SetIndicator();
		}
		else
		{
			PlayMontageAndWait = CreatePlayMontageAndWaitTask(0, 1, "Attack");
		}

		if (PlayMontageAndWait && WaitGameplayEvent)
		{
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Sevarog_Q::EndAbilityCallback);
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Sevarog_Q::EndAbilityCallback);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Sevarog_Q::EndAbilityCallback);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Sevarog_Q::EndAbilityCallback);

			WaitGameplayEvent->EventReceived.AddDynamic(this, &UAbility_Sevarog_Q::AttackPointCallback);

			PlayMontageAndWait->ReadyForActivation();
			WaitGameplayEvent->ReadyForActivation();
		}
		else
		{
			EndAbilityCallback();
		}
	}
}

void UAbility_Sevarog_Q::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	bIsFirstTrigger = true;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAbility_Sevarog_Q::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	SpawnedIndicatorTasks.Reset();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Sevarog_Q::AttackPointCallback(FGameplayEventData Payload)
{
	for (auto Element : SpawnedIndicatorTasks)
	{
		Element->ExternalCancel();
	}
	
	ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

	TArray<AActor*> TargetActors = StartTraceAndGetTargets();

	for (const auto& Target : TargetActors)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Damage_SetByCaller, SkillDamageRate);
		
		FGameplayAbilityTargetDataHandle TargetDataHandle = CreateAbilityTargetDataFromActor(Target);
		
		TArray<FActiveGameplayEffectHandle> AppliedEffectsHandle =
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
				EffectSpecHandle, TargetDataHandle);
	}
}

void UAbility_Sevarog_Q::EndAbilityCallback()
{
	bIsFirstTrigger = true;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_Sevarog_Q::SetIndicator()
{
	for (auto Indicator : TestIndicators)
	{
		UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent = UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(
			this, Indicator, OwnerCharacter, SkillRange * 0.5, FVector{ 1, SkillSideRange * 0.5, SkillRange * 0.5},
			bAttachLocationToParent, bFollowRotationToParent);
		
		SpawnActorAndFollowParent->ReadyForActivation();
		if (auto SpawnedIndicator = Cast<ASkillIndicator>(SpawnActorAndFollowParent->GetSpawnedActor()))
		{
			if (Cast<AMonsterBase>(OwnerCharacter))
			{
				SpawnedIndicator->SetIndicatorColor(FColor::Red);
				SpawnedIndicatorTasks.Add(SpawnActorAndFollowParent);
			}
			else
			{
				SpawnedIndicator->SetIndicatorColor(FColor::Blue);
			}
		}
	}
}

TArray<AActor*> UAbility_Sevarog_Q::StartTraceAndGetTargets()
{
	TArray<TSubclassOf<AActor>>	FilterClasses;
	
	if (Cast<AMonsterBase>(OwnerCharacter))
	{
		FilterClasses.Add(ARaidBossPlayerBase::StaticClass());
	}
	else
	{
		FilterClasses.Add(AMonsterBase::StaticClass());
	}
	
	return URaidBossLibrary::GetActorsInRectangle(this, OwnerCharacter->GetActorLocation(),
		UKismetMathLibrary::GetForwardVector(FRotator{0, OwnerCharacter->GetControlRotation().Yaw, 0}), SkillRange, SkillSideRange,
		FilterClasses, {}, true, bDrawDebugLine);
}
