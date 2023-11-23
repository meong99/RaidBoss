// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/MonsterBase.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Character/RaidBossAnimBase.h"
#include "Character/Enemy/RaidBossEnemyControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Management/RaidBossGameplayTags.h"

AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	bUseControllerRotationYaw = false;
	
	Tags.Add("Enemy");
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterBase::InitMonster(const FMonsterInfo& MonsterInfoRow)
{
	GetMesh()->SetSkeletalMesh(MonsterInfoRow.SkeletalMesh);
	GetMesh()->SetAnimClass(MonsterInfoRow.AnimBP);
	MonsterType = MonsterInfoRow.MonsterType;
	MonsterSpec = MonsterInfoRow.MonsterSpec;
	MonsterReward = MonsterInfoRow.MonsterReward;
	CurrentPlayerState = EPlayerState::Alive;

	SetAnimationData(MonsterInfoRow.MonsterAnimationTable);
}

void AMonsterBase::ApplyMonsterEffectSpec()
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle OutGoingSpec = AbilitySystemComponent->MakeOutgoingSpec(CharacterStatusEffect, 1, ContextHandle);
		
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MaxHealth, MonsterSpec.MaxHealth);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MaxMana, MonsterSpec.MaxMana);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackPower, MonsterSpec.AttackPower);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackRange, MonsterSpec.AttackRange);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_DefensePower, MonsterSpec.DefencePower);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_MoveSpeed, MonsterSpec.MoveSpeed);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Init_Character_AttackSpeed, MonsterSpec.AttackSpeed);

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*OutGoingSpec.Data);
	}
}

ARaidBossEnemyControllerBase* AMonsterBase::GetRiadBossEnemyController() const
{
	return Cast<ARaidBossEnemyControllerBase>(GetController());
}
