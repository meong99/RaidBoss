// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Aurora/Ability_Aurora_E.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Skill/Indicator/SkillIndicator.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

UAbility_Aurora_E::UAbility_Aurora_E()
{
	SkillDamageRate = 3.f;
}

void UAbility_Aurora_E::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWaitTask = CreatePlayMontageAndWaitTask(0);
	UAbilityTask_WaitGameplayEvent*		WaitGameplayEventTask = CreateWaitGameplayEventTask(
		RaidBossGameplayTags::Get().Animation_Notify_AttackPoint);

	if (bIsFirstTrigger)
	{
		bIsFirstTrigger = false;
		SetIndicator();
	}
	else
	{
		bIsFirstTrigger = true;
		if (PlayMontageAndWaitTask && WaitGameplayEventTask)
		{
			ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
			
			OwnerCharacter->SetCanActivateNormalAttack(false);
			OwnerCharacter->SetIsMovementBlocked(true);
			
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UAbility_Aurora_E::ToEndAbilityCallback);
			PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UAbility_Aurora_E::ToEndAbilityCallback);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UAbility_Aurora_E::ToEndAbilityCallback);
			PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UAbility_Aurora_E::ToEndAbilityCallback);
	
			WaitGameplayEventTask->EventReceived.AddDynamic(this, &UAbility_Aurora_E::AttackPointEventCallback);
	
			PlayMontageAndWaitTask->ReadyForActivation();
			WaitGameplayEventTask->ReadyForActivation();
		}
		else
		{
			ToEndAbilityCallback();
		}
	}
}

void UAbility_Aurora_E::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	bIsFirstTrigger = true;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UAbility_Aurora_E::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (OwnerCharacter)
	{
		OwnerCharacter->SetCanActivateNormalAttack(true);
		OwnerCharacter->SetIsMovementBlocked(false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_Aurora_E::AttackPointEventCallback(FGameplayEventData EventData)
{
	SpawnParticles();
	AttackTargetsInRadius();
}

void UAbility_Aurora_E::ToEndAbilityCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_Aurora_E::SetIndicator()
{
	for (auto Indicator : TestIndicators)
	{
		UAT_SpawnActorAndFollowParent* SpawnActorAndFollowParent = UAT_SpawnActorAndFollowParent::SpawnActorAndFollowParent(
			this, Indicator, OwnerCharacter, 0, FVector{ 1, SkillRadius, SkillRadius},
			bAttachLocationToParent, bFollowRotationToParent);
		
		if (SpawnActorAndFollowParent == nullptr)
		{
			continue;
		}
		SpawnActorAndFollowParent->ReadyForActivation();
		
		if (auto SpawnedIndicator = Cast<ASkillIndicator>(SpawnActorAndFollowParent->GetSpawnedActor()))
		{
			SpawnedIndicator->SetIndicatorColor(FColor::Blue);
		}
	}
}

void UAbility_Aurora_E::SpawnParticles() const
{
	FVector OwnerForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector OwnerWorldLocation = OwnerCharacter->GetActorLocation();
	float SpawnAngle = 360.f / ParticleCount;
	
	for (int i = 0; i < ParticleCount; i++)
	{
		FVector SpawnDirection = URaidBossLibrary::RotateVector(OwnerForwardVector, FRotator(0, SpawnAngle * i, 0));
		FVector	SpawnLocation = OwnerWorldLocation + SpawnDirection * SkillRadius;

		SpawnLocation = URaidBossLibrary::GetFloorLocation(this, SpawnLocation);
		URaidBossLibrary::SpawnEmitterAtLocation(GetWorld(), Particle,
			FTransform{FRotator{0, OwnerCharacter->GetActorRotation().Yaw + SpawnAngle * i, 0}, SpawnLocation});
	}
}

void UAbility_Aurora_E::AttackTargetsInRadius() const
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
	
	FVector OwnerForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector OwnerWorldLocation = OwnerCharacter->GetActorLocation();
	
	TArray<AActor*> TargetActors = URaidBossLibrary::GetActorsInAngle(this, OwnerWorldLocation, OwnerForwardVector, SkillRadius,
		FilterClasses, {}, 360);

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
