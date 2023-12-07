// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

class ARaidBossCharacterBase;
/**
 * 
 */
UCLASS()
class RAIDBOSS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void	SetWidgetOwner(ARaidBossCharacterBase* InWidgetOwner) { WidgetOwner = InWidgetOwner; };
	
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | Widget")
	void	CallToNotifyNewDamage(float NewDamage);

	UFUNCTION(BlueprintImplementableEvent, Category="Raid Boss | Widget")
	void	OnDamageBarChanged();
	
protected:
	/*
	 *	Changed on Initialization * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	
	//
	
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	TObjectPtr<ARaidBossCharacterBase>	WidgetOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Widget", meta=(BindWidget))
	TObjectPtr<UProgressBar>	HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Raid Boss | Widget", meta=(BindWidget))
	TObjectPtr<UProgressBar>	DamageBar;

	// 데미지 바 감소 딜레이
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	DamageBarDelayTime = 1;
	
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	DamageBarInterSpeed = 5;
	/*
	 *	Changed in cycle * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 */
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	CurrentHealth = 0;

	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	CurrentMaxHealth = 0;
	
	// 마지막 타격으로부터의 시간
	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	TimeSinceLastDamage = 0;

	UPROPERTY(BlueprintReadOnly, Category="Raid Boss | Widget")
	float	HealthBeforeDamaged = 0;
};
