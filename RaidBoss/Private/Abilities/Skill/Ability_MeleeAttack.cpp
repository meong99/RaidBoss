// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/Ability_MeleeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Skill/Indicator/FanShapeIndicator.h"
#include "Abilities/Task/AT_SpawnActorAndFollowParent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/RaidBossCharacterBase.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/RaidBossPlayerBase.h"
#include "Equipment/Weapon/Weapon.h"
#include "Management/RaidBossGameplayTags.h"
#include "Util/RaidBossLibrary.h"

bool UAbility_MeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                              const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool	bIsValidOwner = OwnerCharacter ? true : false;
	bool	bIsValidWeaponData = CurrentWeapon != nullptr;

	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return	bCanActivate &&
			bIsValidOwner &&
			bIsValidWeaponData &&
			bCanActivateNextAttack;
}

void UAbility_MeleeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	CurrentWeapon = Cast<AWeapon>(Spec.SourceObject);

	if (OwnerCharacter)
	{
		SkillRange = OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackRange();
	}

	MaximumCombo = CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims.Num();
}

void UAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (OwnerCharacter)
	{
		SkillRange = OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackRange();
	}

	SpawnedIndicatorTasks.Reset();
	
	if (CurrentCombo == MaximumCombo)
	{
		CurrentCombo = ComboInitValue;
	}

	UAbilityTask_PlayMontageAndWait*	PlayMontageAndWait = nullptr;
	
	if (CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims.IsValidIndex(CurrentCombo))
	{
		float AttackSpeed = OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackSpeed();
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, FName(GetName()), CurrentWeapon->GetWeaponData().WeaponAnimations.UseAnims[CurrentCombo], 1 * AttackSpeed);
	}
	
	CurrentCombo++;
	bCanActivateNextAttack = false;
	
	UAbilityTask_WaitGameplayEvent* WaitCanActivateNextAttackEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_CanActivateNextAttack, OwnerCharacter, true);
			
	UAbilityTask_WaitGameplayEvent* WaitAttackPointEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_AttackPoint, OwnerCharacter, true);
	
	UAbilityTask_WaitGameplayEvent* WaitResetComboEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, RaidBossGameplayTags::Get().Animation_Notify_ComboReset, OwnerCharacter, true);
	
	if (PlayMontageAndWait && WaitAttackPointEvent && WaitResetComboEvent && WaitCanActivateNextAttackEvent)
	{
		SetIndicator();
		
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UAbility_MeleeAttack::NotifyMontageCanceledCallBack);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UAbility_MeleeAttack::NotifyMontageCanceledCallBack);
		WaitAttackPointEvent->EventReceived.AddDynamic(this,&UAbility_MeleeAttack::NotifyAttackPointComesCallBack);
		WaitResetComboEvent->EventReceived.AddDynamic(this,	&UAbility_MeleeAttack::NotifyComboResetCallBack);
		WaitCanActivateNextAttackEvent->EventReceived.AddDynamic(this,	&UAbility_MeleeAttack::NotifyCanActivateNextAttackCallBack);
		
		WaitAttackPointEvent->ReadyForActivation();
		WaitResetComboEvent->ReadyForActivation();
		WaitCanActivateNextAttackEvent->ReadyForActivation();
		PlayMontageAndWait->ReadyForActivation();
	}
	else
	{
		NotifyMontageCanceledCallBack();
	}
}

void UAbility_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	bCanActivateNextAttack = true;
	SpawnedIndicatorTasks.Reset();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

float UAbility_MeleeAttack::GetSkillRange() const
{
	float CurrentSkillRange = 1;
	
	if (OwnerCharacter)
	{
		CurrentSkillRange = OwnerCharacter->GetCharacterStatusAttributeSet()->GetAttackRange();
	}
	
	return CurrentSkillRange;
}

void UAbility_MeleeAttack::SetIndicator()
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

void UAbility_MeleeAttack::NotifyMontageCanceledCallBack()
{
	bCanActivateNextAttack = true;
	
	if (Cast<ARaidBossPlayerBase>(OwnerCharacter))
	{
		CurrentCombo = ComboInitValue;
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAbility_MeleeAttack::NotifyAttackPointComesCallBack(FGameplayEventData Payload)
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
	
	TArray<AActor*> TargetActors = URaidBossLibrary::GetActorsInAngle(
		this, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorForwardVector(),
		SkillRange, FilterClasses, {}, SkillAngle, true, bDrawDebugLine);
	
	ApplyEffectToTarget(TargetActors);
}

void UAbility_MeleeAttack::NotifyComboResetCallBack(FGameplayEventData Payload)
{
	if (Cast<ARaidBossPlayerBase>(OwnerCharacter))
	{
		CurrentCombo = ComboInitValue;
	}
	NotifyMontageCanceledCallBack();
}

void UAbility_MeleeAttack::NotifyCanActivateNextAttackCallBack(FGameplayEventData Payload)
{
	bCanActivateNextAttack = true;
	OwnerCharacter->SetIsMovementBlocked(false);
}

void UAbility_MeleeAttack::ApplyEffectToTarget(const TArray<AActor*>& TargetActors)
{
	for (auto Element : SpawnedIndicatorTasks)
	{
		Element->ExternalCancel();
	}
	
	for (const auto& TargetActor : TargetActors)
	{
		ARaidBossCharacterBase* TargetCharacter = Cast<ARaidBossCharacterBase>(TargetActor);
		
		if (TargetCharacter == nullptr && TargetCharacter->GetCurrentCharacterState() != ECharacterState::Alive)
		{
			continue;
		}
		
		FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor);
		
		TArray<FActiveGameplayEffectHandle> EffectHandles =
			ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, EffectClass, 1);

		OnHitTarget(TargetActor);
	}
}
