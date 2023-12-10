// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HealthBarWidget.h"
#include "Abilities/RaidBossCharacterStatusAttributeSet.h"
#include "Character/RaidBossCharacterBase.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetOwner)
	{
		HealthBeforeDamaged = WidgetOwner->GetCharacterStatusAttributeSet()->GetHealth();
	}
	else
	{
		WidgetOwner = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());
	}
}

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	const URaidBossCharacterStatusAttributeSet*	AttributeSet = WidgetOwner ? WidgetOwner->GetCharacterStatusAttributeSet() : nullptr;
		
	if (AttributeSet == nullptr)
	{
		return;
	}

	CurrentHealth = AttributeSet->GetHealth();
	CurrentMaxHealth = AttributeSet->GetMaxHealth();

	TimeSinceLastDamage += InDeltaTime;
	
	if (bool bHaveToSetDamageBar = TimeSinceLastDamage >= DamageBarDelayTime)
	{
		DamageBar->SetPercent(FMath::FInterpTo(DamageBar->GetPercent(), HealthBar->GetPercent(), InDeltaTime, DamageBarInterSpeed));

		OnDamageBarChanged();
	}

	HealthBar->SetPercent(CurrentHealth / CurrentMaxHealth);

	if (bool bTakenNewDamage = CurrentHealth < HealthBeforeDamaged)
	{
		float NewDamage = HealthBeforeDamaged - CurrentHealth;
		
		HealthBeforeDamaged = CurrentHealth;

		TimeSinceLastDamage = 0;

		CallToNotifyNewDamage(NewDamage);
	}
}
