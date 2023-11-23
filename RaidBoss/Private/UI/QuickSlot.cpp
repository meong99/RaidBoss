// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuickSlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/Image.h"

UQuickSlot::UQuickSlot()
{
}

void UQuickSlot::ActivateThisSlot()
{
	if (OwningSlot)
	{
		OwningSlot->ActivateThisSlot();
	}
	else if (AbilityTriggerTag.IsValid())
	{
		SendGameplayEventWithTag();
	}
}

bool UQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (InOperation == nullptr)
	{
		return false;
	}
	URaidBossSlotWidget*	OtherSlot = Cast<URaidBossSlotWidget>(InOperation->Payload);

	if (OtherSlot == nullptr)
	{
		return false;
	}

	SetOtherSlot(OtherSlot);
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UQuickSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
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

void UQuickSlot::SetOtherSlot(URaidBossSlotWidget* OtherSlot)
{
	OwningSlot = OtherSlot;
	SetTexture(OwningSlot->GetTexture());
}

void UQuickSlot::RemoveOtherSlot()
{
	OwningSlot = nullptr;
	SetTexture(nullptr);
}

void UQuickSlot::SetNewSkill(const URaidBossSkillBase* NewSkill)
{
	if (NewSkill)
	{
		AbilityTriggerTag = NewSkill->GetAbilityTriggerTag();
		SetTexture(NewSkill->GetSkillInfo().SkillTexture);
	}
}

void UQuickSlot::RemoveRegisteredSkill()
{
	AbilityTriggerTag = FGameplayTag{};
	SetTexture(nullptr);
}

void UQuickSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwningSlot)
	{
		if (OwningSlot->GetAbilityTriggerTag().IsValid() == false)
		{
			RemoveOtherSlot();
		}
	}
}

void UQuickSlot::SendGameplayEventWithTag(const FGameplayTag& TagToAdd) const
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
