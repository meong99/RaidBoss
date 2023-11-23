// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "Equipment/Weapon/WeaponInterface.h"
#include "Item/WeaponDataAsset.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class URaidBossSkillBase;

UCLASS()
class RAIDBOSS_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	AWeapon();
	
protected:
	virtual void	BeginPlay() override;

public:
	virtual bool	Attack() override;
	
	void			LoadWeaponData(FWeaponKey InWeaponKey);
	
	void			NotifyNewWeaponEquipped();
	
	void			ClearWeaponData();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Weapon")
	void			AddWidgetToViewport();
	UFUNCTION(BlueprintCallable, Category="Raid Boss | Weapon")
	void			RemoveWidgetFromViewport();

protected:
	URaidBossAbilitySystemComponent*	GetRaidBossAbilitySystemComponent() const;
	
private:
	const TCHAR*	GetDataTableLink(FWeaponKey InWeaponKey) const;
	void			GiveAbilityToAsc();
	void			AttachToOwner();
	void			DetachToOwner();
	void			ApplyWeaponStatToOwner();
	void			GetAnimDataFromOwner();
	
public:
	/*
	 *	Access
	 */
	
	virtual AWeapon*					GetCurrentObject() override { return this; }
	virtual FWeaponKey					GetWeaponKey() const override { return WeaponData.WeaponKey; }
	virtual const FWeaponData&			GetWeaponData() const override { return WeaponData; }
	USkeletalMeshComponent*				GetSkeletalMesh() const { return SkeletalMeshComponent; }
	TArray<FGameplayAbilitySpecHandle>	GetAbilitySpecHandles() const { return AbilitySpecHandles; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	USkeletalMeshComponent*	SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Raid Boss | Item")
	FWeaponData	WeaponData;

	UPROPERTY()
	TArray<URaidBossSkillBase*>			CurrentSkills;
	
	TArray<FGameplayAbilitySpecHandle>	AbilitySpecHandles;
	FActiveGameplayEffectHandle			AppliedEffectHandle;
	TArray<UUserWidget*>	WidgetInstances;
};
