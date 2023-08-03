#include "Abilities/Player/RaidBossSerathESkill.h"
#include "Components/CapsuleComponent.h"
#include "Character/Enemy/RaidBossEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

URaidBossSerathESkill::URaidBossSerathESkill()
{
	SkillInfo.SkillCost = 0;
	SkillInfo.SkillLevel = 0;
	SkillInfo.MaximumSkillLevel = 5;
	SkillInfo.MinimumSkillLevel = 0;
}

void URaidBossSerathESkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false || IsValid(OwnerCharacter) == false)
	{
		EndAbilityCallback();
		return;
	}
	
	const auto		PlayMontageTask = CreatePlayMontageAndWaitTask();
	FGameplayTag	ThisTag = FGameplayTag::RequestGameplayTag(FName("Skill.E"));
	const auto		WaitEventTask = CreateWaitGameplayEventTask(ThisTag);
	
	if (::IsValid(PlayMontageTask) == true)
	{
		BlockOwnerCharacterMovement();
		
		PlayMontageTask->OnCompleted.AddDynamic(this, &URaidBossSerathESkill::EndAbilityCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URaidBossSerathESkill::EndAbilityCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URaidBossSerathESkill::EndAbilityCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URaidBossSerathESkill::EndAbilityCallback);
		PlayMontageTask->Activate();

		WaitEventTask->EventReceived.AddDynamic(this, &URaidBossSerathESkill::EventReceivedCallback);
		WaitEventTask->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Ability can not activate"));
		EndAbilityCallback();
	}
}

void URaidBossSerathESkill::EventReceivedCallback(const FGameplayEventData Payload)
{
	if (IsValid(OwnerCharacter) == false)
		return;

	TArray<AActor*>	OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaidBossEnemyBase::StaticClass(), OutArray);

	for (const auto Element : OutArray)
	{
		ARaidBossEnemyBase* EnermyObject = Cast<ARaidBossEnemyBase>(Element);


		FVector	OwningCharacterLocation = OwnerCharacter->GetActorLocation();
		FVector	EnermyLocation = EnermyObject->GetActorLocation();
		float	EnermyCapsuleSize = EnermyObject->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (FVector::Dist(OwningCharacterLocation, EnermyLocation) <= Range + EnermyCapsuleSize)
		{
			EnermyArr.Add(EnermyObject);
		}
	}

	for (const auto EnermyObject : EnermyArr)
	{
		FGameplayAbilityTargetDataHandle	TargetData = CreateAbilityTargetDataFromActor(EnermyObject);

		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, EffectClass, 1);
	}
	FVector	Location = OwnerCharacter->GetActorLocation();
	Location.Z = 0;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Location);
}

void URaidBossSerathESkill::EndAbilityCallback()
{
	ReleaseOwnerCharacterMovement();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
