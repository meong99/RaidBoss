// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Weapon/Weapon.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Blueprint/UserWidget.h"
#include "Character/RaidBossCharacterBase.h"
#include "Engine/DataTable.h"
#include "Management/RaidBossGameplayTags.h"

AWeapon::AWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh Component"));

	GetSkeletalMesh()->SetRenderCustomDepth(true);
}

bool AWeapon::Attack()
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	
	if (AbilitySystemComponent)
	{
		FGameplayEventData Payload;
		
		const FGameplayTag BasicAttackTag = RaidBossGameplayTags::Get().Attack_BasicAttack;
		
		return AbilitySystemComponent->HandleGameplayEvent(BasicAttackTag, &Payload) > 0 ? true : false;
	}

	return false;
}

void AWeapon::LoadWeaponData(FWeaponKey InWeaponKey)
{
	const TCHAR* DT_WeaponDataLink = GetDataTableLink(InWeaponKey);
	const UDataTable* DT_ItemData = DT_WeaponDataLink ? LoadObject<UDataTable>(nullptr, DT_WeaponDataLink) : nullptr;
	FString RowName = FString::FromInt(InWeaponKey.WeaponNumber);
	const FWeaponData* WeaponDataRow = DT_ItemData ? DT_ItemData->FindRow<FWeaponData>(*RowName, FString()) : nullptr;
	
	if (WeaponDataRow)
	{
		WeaponData = *WeaponDataRow;
		SkeletalMeshComponent->SetSkeletalMesh(WeaponData.SkeletalMesh);

		FinishSpawning(FTransform::Identity, true);
		GiveAbilityToAsc();
		ApplyWeaponStatToOwner();
		AttachToOwner();
	}
}

void AWeapon::AddWidgetToViewport()
{
	for (auto Widget : WeaponData.WidgetsToApply)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);

		if (NewWidget)
		{
			NewWidget->AddToViewport();
			WidgetInstances.Add(NewWidget);
		}
	}
}

void AWeapon::RemoveWidgetFromViewport()
{
	for (auto Widget : WidgetInstances)
	{
		Widget->RemoveFromParent();
	}
}

void AWeapon::NotifyNewWeaponEquipped()
{
	ARaidBossCharacterBase*	OwnerCharacter = Cast<ARaidBossCharacterBase>(Owner);

	if (OwnerCharacter)
	{
		OwnerCharacter->NotifyNewWeaponEquipped.Broadcast(CurrentSkills);
	}
}

void AWeapon::ClearWeaponData()
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();

	if (AbilitySystemComponent)
	{
		for (const auto& SpecHandle : AbilitySpecHandles)
		{
			AbilitySystemComponent->ClearAbility(SpecHandle);
			AbilitySystemComponent->RemoveActiveGameplayEffect(AppliedEffectHandle);
		}
	}

	RemoveWidgetFromViewport();
	DetachToOwner();
	
	AbilitySpecHandles.Reset();
	WeaponData = FWeaponData();
	SkeletalMeshComponent->SetSkeletalMesh(nullptr);
}

URaidBossAbilitySystemComponent* AWeapon::GetRaidBossAbilitySystemComponent() const
{
	const ARaidBossCharacterBase*	OwnerCharacter = Cast<ARaidBossCharacterBase>(Owner);

	return OwnerCharacter ? OwnerCharacter->GetRaidBossAbilitySystemComponent() : nullptr;
}

const TCHAR* AWeapon::GetDataTableLink(FWeaponKey InWeaponKey) const
{
	if (InWeaponKey.WeaponType == EWeaponType::Monster)
	{
		return TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_MonsterWeaponData.DT_MonsterWeaponData'");
	}
	
	return TEXT("/Script/Engine.DataTable'/Game/Data/DataTables/DT_WeaponData.DT_WeaponData'");
}

void AWeapon::GiveAbilityToAsc()
{
	URaidBossAbilitySystemComponent*	AbilitySystemComponent = GetRaidBossAbilitySystemComponent();
	
	if (AbilitySystemComponent)
	{
		for (const auto Ability : WeaponData.Abilities)
		{
			FGameplayAbilitySpec abilitySpec(Ability.GetDefaultObject(), 1);
			abilitySpec.SourceObject = this;
			
			AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(abilitySpec));
			
			URaidBossAbilityBase* AbilityCDO = Ability->GetDefaultObject<URaidBossAbilityBase>();

			if (URaidBossSkillBase* NewSkill = Cast<URaidBossSkillBase>(AbilityCDO))
			{
				if (NewSkill->GetAbilityTriggerTag() == RaidBossGameplayTags::Get().Attack_BasicAttack ||
					NewSkill->GetAbilityTriggerTag() == RaidBossGameplayTags::Get().Attack_RangeAttack)
				{
					continue;
				}
				
				CurrentSkills.Add(NewSkill);
			}
		}

		NotifyNewWeaponEquipped();
	}
}

void AWeapon::AttachToOwner()
{
	ARaidBossCharacterBase*	OwnerCharacter = Cast<ARaidBossCharacterBase>(Owner);

	if (OwnerCharacter)
	{
		USceneComponent* AttachTarget = OwnerCharacter->GetMesh();
		const FName AttachSocket = *UEnum::GetDisplayValueAsText(WeaponData.AttachSocket).ToString();
		
		AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, AttachSocket);
	}
}

void AWeapon::DetachToOwner()
{
	ARaidBossCharacterBase*	OwnerCharacter = Cast<ARaidBossCharacterBase>(Owner);

	if (OwnerCharacter)
	{
		USceneComponent* AttachTarget = OwnerCharacter->GetMesh();
		
		DetachAllSceneComponents(AttachTarget, FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
	}
}

void AWeapon::ApplyWeaponStatToOwner()
{
	URaidBossAbilitySystemComponent* AbilitySystemComponent = GetRaidBossAbilitySystemComponent();

	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle OutGoingSpec = AbilitySystemComponent->MakeOutgoingSpec(WeaponData.AdditiveEffectToCharacter, 1, ContextHandle);

		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackPower, WeaponData.AttackPower);
		OutGoingSpec.Data->SetSetByCallerMagnitude(RaidBossGameplayTags::Get().Character_Stat_AttackRange, WeaponData.AttackRange);

		AppliedEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*OutGoingSpec.Data);
	}
}
