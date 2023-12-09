// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/Sevarog/Ability_Sevarog_R.h"
#include "Abilities/Skill/SkillIndicator.h"
#include "Abilities/Skill/Indicator/FanShapeIndicator.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/DataPassingObject.h"
#include "Util/RaidBossLibrary.h"

UAbility_Sevarog_R::UAbility_Sevarog_R()
{
	SkillDamageRate = 5.f;
}

void UAbility_Sevarog_R::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bIsFirstTrigger && Cast<AMonsterBase>(OwnerCharacter) == nullptr)
	{
		SetIndicator();
		bIsFirstTrigger = false;
	}
	else
	{
		bIsFirstTrigger = true;
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait;
		auto WaitGameplayEvent = CreateWaitGameplayEventTask(RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, false);

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
			ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
			
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_Sevarog_R::EndAbilityCallback);
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UAbility_Sevarog_R::EndAbilityCallback);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_Sevarog_R::EndAbilityCallback);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UAbility_Sevarog_R::EndAbilityCallback);
	
			WaitGameplayEvent->EventReceived.AddDynamic(this, &UAbility_Sevarog_R::AttackPointCallback);
	
			PlayMontageAndWait->ReadyForActivation();
			WaitGameplayEvent->ReadyForActivation();
		}
		else
		{
			EndAbilityCallback();
		}
	}
}

void UAbility_Sevarog_R::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	bIsFirstTrigger = true;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAbility_Sevarog_R::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	SpawnedIndicatorTasks.Reset();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Sevarog_R::SetIndicator()
{
	for (auto Indicator : TestIndicators)
	{
		UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent = UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(
			this, Indicator, OwnerCharacter, AdditiveIndicatorLocation, FVector{ 1, SkillRange, SkillRange },
			bAttachLocationToParent, bFollowRotationToParent);
		
		SpawnActorAndFollowParent->ReadyForActivation();
		
		auto FanShapeIndicator = Cast<AFanShapeIndicator>(SpawnActorAndFollowParent->GetSpawnedActor());
		if (FanShapeIndicator)
		{
			FanShapeIndicator->SetIndicatorValue(SkillAngle);
			if (Cast<AMonsterBase>(OwnerCharacter))
			{
				FanShapeIndicator->SetIndicatorColor(FColor::Red);
				SpawnedIndicatorTasks.Add(SpawnActorAndFollowParent);
			}
			else
			{
				FanShapeIndicator->SetIndicatorColor(FColor::Blue);
			}
		}
	}
}

void UAbility_Sevarog_R::AttackPointCallback(FGameplayEventData Payload)
{
	for (auto Element : SpawnedIndicatorTasks)
	{
		Element->ExternalCancel();
	}
	
	TArray<TSubclassOf<AActor>>	FilterClasses;
	
	if (Cast<AMonsterBase>(OwnerCharacter))
	{
		FilterClasses.Add(ARaidBossPlayerBase::StaticClass());
	}
	else
	{
		FilterClasses.Add(AMonsterBase::StaticClass());
	}
	
	TArray<AActor*>	TargetActors = URaidBossLibrary::GetActorsInAngle(this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorForwardVector(),
		SkillRange, FilterClasses, {}, SkillAngle, true, bDrawDebugLine);

	UDataPassingObject*	DataPassingObject = NewObject<UDataPassingObject>();

	if (DataPassingObject)
	{
		DataPassingObject->FloatValue.Add("KnockBackPower", KnockBackPower);
		DataPassingObject->VectorValue.Add("Direction", OwnerCharacter->GetActorForwardVector());
	}
	
	for (const auto& Target : TargetActors)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Damage_SetByCaller, SkillDamageRate);
		
		FGameplayAbilityTargetDataHandle TargetDataHandle = CreateAbilityTargetDataFromActor(Target);
		
		TArray<FActiveGameplayEffectHandle> AppliedEffectsHandle =
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
				EffectSpecHandle, TargetDataHandle);

		FGameplayEventData	EventData;

		EventData.Instigator = OwnerCharacter;
		EventData.OptionalObject = DataPassingObject;
		
		SendGameplayEventToActor(Target, RaidBossGameplayTags::Get().StatusEffect_KnockBack, EventData);
	}
}

void UAbility_Sevarog_R::EndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
