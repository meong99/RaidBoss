// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class URaidBossSkillBase;

UCLASS()
class RAIDBOSS_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	AWeapon();
	
public:
	virtual bool	Attack() override;

	void	LoadWeaponData(FWeaponKey InWeaponKey);
	
	void	AddWidgetToViewport();
	
	void	RemoveWidgetFromViewport();
	
	void	NotifyNewWeaponEquipped();
	
	void	ClearWeaponData();

	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	
	/*
	 *	Access Method * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	virtual AWeapon*					GetCurrentObject() override { return this; }
	virtual FWeaponKey					GetWeaponKey() const override { return WeaponData.WeaponKey; }
	virtual const FWeaponData&			GetWeaponData() const override { return WeaponData; }
	USkeletalMeshComponent*				GetSkeletalMesh() const { return SkeletalMeshComponent; }
	TArray<FGameplayAbilitySpecHandle>	GetAbilitySpecHandles() const { return AbilitySpecHandles; }
	
protected:
	const TCHAR*	GetDataTableLink(FWeaponKey InWeaponKey) const;
	
	void			GiveAbilityToAsc();
	
	void			AttachToOwner();
	
	void			DetachToOwner();

	void			ApplyWeaponStatToOwner();
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	USkeletalMeshComponent*		SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	FWeaponData					WeaponData;

	UPROPERTY()
	TArray<URaidBossSkillBase*>	CurrentSkills;

	UPROPERTY()
	TArray<UUserWidget*>		WidgetInstances;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	TArray<FGameplayAbilitySpecHandle>	AbilitySpecHandles;
	
	FActiveGameplayEffectHandle			AppliedEffectHandle;
};
