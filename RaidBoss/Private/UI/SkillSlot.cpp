// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SkillSlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/Image.h"
#include "Management/RaidBossGameplayTags.h"

void USkillSlot::ActivateThisSlot()
{
	TryActivateAbilityInSlot();
}

void USkillSlot::TryActivateAbilityInSlot() const
{
	SendGameplayEventWithTag();
}

void USkillSlot::InitSlot(FGameplayTag InAbilityTriggerTag, UTexture2D* InSkillTexture, FString InSkillName, int32 InRequirePoint, int32 InSkillLevel)
{
	AbilityTriggerTag = InAbilityTriggerTag;
	SetTexture(InSkillTexture);
	CurrentSkillName->SetText(FText::FromString(InSkillName));
	RequirePoint->SetText(FText::FromString(FString::FromInt(InRequirePoint)));
	CurrentSkillLevel->SetText(FText::FromString(FString::FromInt(InSkillLevel)));
}

void USkillSlot::NotifySkillLevelChangedCallback(FGameplayTag InAbilityTriggerTag, int32 InCurrentSkillLevel)
{
	if (InAbilityTriggerTag == AbilityTriggerTag)
	{
		CurrentSkillLevel->SetText(FText::FromString(FString::FromInt(InCurrentSkillLevel)));
	}
}

void USkillSlot::IncreaseSkillLevel()
{
	SendGameplayEventWithTag(RaidBossGameplayTags::Get().Event_Skill_IncreaseLevel);
}

void USkillSlot::DecreaseSkillLevel()
{
	SendGameplayEventWithTag(RaidBossGameplayTags::Get().Event_Skill_DecreaseLevel);
}

void USkillSlot::AddSkillLevelMonitoring() const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		CharacterBase->NotifySkillLevelChanged.AddDynamic(this, &USkillSlot::NotifySkillLevelChangedCallback);
	}
}

void USkillSlot::RemoveSkillLevelMonitoring() const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		CharacterBase->NotifySkillLevelChanged.RemoveDynamic(this, &USkillSlot::NotifySkillLevelChangedCallback);
	}
}

void USkillSlot::SendGameplayEventWithTag(const FGameplayTag& TagToAdd/* = FGameplayTag{}*/) const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());
	
	if (CharacterBase && AbilityTriggerTag.IsValid())
	{
		FGameplayEventData	EventData;

		if (TagToAdd.IsValid())
		{
			EventData.InstigatorTags.AddTag(TagToAdd);
		}
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CharacterBase, AbilityTriggerTag, EventData);
	}
}

void USkillSlot::NativeOnInitialized()
{
	LevelUpButton->OnClicked.AddDynamic(this, &USkillSlot::IncreaseSkillLevel);
	LevelDownButton->OnClicked.AddDynamic(this, &USkillSlot::DecreaseSkillLevel);
	AddSkillLevelMonitoring();
}

void USkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                      UDragDropOperation*& OutOperation)
{
	if (AbilityTriggerTag.IsValid())
	{
		BindImage->SetDesiredSizeOverride(FVector2d(BindImage->Brush.ImageSize.X, BindImage->Brush.ImageSize.Y));
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
		OutOperation->Payload = this;
		OutOperation->DefaultDragVisual = BindImage;
		OutOperation->Pivot = DragPivot;
		OutOperation->Offset = DragOffset;
	}
}

FReply USkillSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AbilityTriggerTag.IsValid())
	{
		TryActivateAbilityInSlot();
	}
	
	return UWidgetBlueprintLibrary::Handled().NativeReply;
}
