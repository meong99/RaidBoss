// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuickSlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/RaidBossAbilitySystemComponent.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/Image.h"

UQuickSlot::UQuickSlot()
{
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
		else if (ARaidBossCharacterBase* CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn()))
		{
			URaidBossAbilitySystemComponent*	AbilitySystemComponent = CharacterBase->GetRaidBossAbilitySystemComponent();

			if (AbilitySystemComponent)
			{
				auto AbilityMap = AbilitySystemComponent->GetInstanceAbilitiesByTag();

				URaidBossAbilityBase*	AbilityBase = AbilityMap.FindRef(OwningSlot->GetAbilityTriggerTag());

				if (AbilityBase)
				{
					AbilityBase->GetCooldownTimeRemainingAndDuration(AbilityBase->GetCurrentAbilitySpecHandle(),
						AbilityBase->GetCurrentActorInfo(), RemainingCoolDown, OriginCoolDown);
				}
				else
				{
					RemainingCoolDown = 0;
					OriginCoolDown = 0;
				}
			}
		}
	}
	else if (ARaidBossCharacterBase* CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn()))
	{
		URaidBossAbilitySystemComponent*	AbilitySystemComponent = CharacterBase->GetRaidBossAbilitySystemComponent();

		if (AbilitySystemComponent && AbilityTriggerTag.IsValid())
		{
			auto AbilityMap = AbilitySystemComponent->GetInstanceAbilitiesByTag();

			URaidBossAbilityBase*	AbilityBase = AbilityMap.FindRef(AbilityTriggerTag);

			if (AbilityBase)
			{
				AbilityBase->GetCooldownTimeRemainingAndDuration(AbilityBase->GetCurrentAbilitySpecHandle(),
						AbilityBase->GetCurrentActorInfo(), RemainingCoolDown, OriginCoolDown);
			}
			else
			{
				RemainingCoolDown = 0;
				OriginCoolDown = 0;
			}
		}
	}
	
	if (RemainingCoolDown && OriginCoolDown)
	{
		bShouldPlayFinishAnimation = true;
		CooldownBar->SetPercent(RemainingCoolDown / OriginCoolDown);
		CoolTime->SetText(FText::FromString(FString::FromInt(RemainingCoolDown)));
		CooldownBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CoolTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (RemainingCoolDown <= 0 && bShouldPlayFinishAnimation)
	{
		PlayAnimation(CooldownFinishedAnimation);
		bShouldPlayFinishAnimation = false;
		CoolTime->SetVisibility(ESlateVisibility::Hidden);
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

void UQuickSlot::SendGameplayEventWithTag(const FGameplayTag& TagToAdd)
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
