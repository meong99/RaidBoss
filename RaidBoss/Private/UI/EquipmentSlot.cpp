// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EquipmentSlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/Image.h"
#include "Management/RaidBossGameplayTags.h"
#include "UI/InventorySlot.h"

void UEquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BackgroundImage->SetBrushFromTexture(DefaultTexture);

	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase && !AbilityTriggerTag.IsValid())
	{
		CharacterBase->NotifyEquipmentChanged.AddDynamic(this, &UEquipmentSlot::NotifyEquipmentChangedCallback);
	}
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (InOperation == nullptr)
	{
		return false;
	}
	
	if (UInventorySlot* OtherInventorySlot = Cast<UInventorySlot>(InOperation->Payload))
	{
		OtherInventorySlot->ActivateThisSlot();
	}
	
	return true;
}

void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
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

FReply UEquipmentSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	
	if (AbilityTriggerTag.IsValid())
	{
		UnEquipItem();
	}
	
	return UWidgetBlueprintLibrary::Handled().NativeReply;
}

void UEquipmentSlot::NotifyEquipmentChangedCallback(FGameplayTag InAbilityTriggerTag, int32 InEquipmentType, UTexture2D* NewItemImage)
{
	if (static_cast<int32>(EquipmentType) != InEquipmentType)
	{
		return;
	}
	
	if (InAbilityTriggerTag.IsValid() && NewItemImage)
	{
		if (AbilityTriggerTag.IsValid() && InAbilityTriggerTag != AbilityTriggerTag)
		{
			UnEquipItem();
		}
		
		EquipItem(InAbilityTriggerTag, NewItemImage);
	}
	else
	{
		UnEquipItem();
	}
}

void UEquipmentSlot::EquipItem(FGameplayTag InAbilityTriggerTag, UTexture2D* NewItemImage)
{
	AbilityTriggerTag = InAbilityTriggerTag;
	BindImage->SetBrushFromTexture(NewItemImage);
	BindImage->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentSlot::UnEquipItem()
{
	RetriggerAbilityForUnEquipItem();
	AbilityTriggerTag = FGameplayTag{};
	BindImage->SetVisibility(ESlateVisibility::Hidden);
}

void UEquipmentSlot::RetriggerAbilityForUnEquipItem() const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase && AbilityTriggerTag.IsValid())
	{
		FGameplayEventData	EventData;

		EventData.InstigatorTags.AddTag(RaidBossGameplayTags::Get().Event_Equipment_ToRemove);
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CharacterBase, AbilityTriggerTag, EventData);
	}
}
