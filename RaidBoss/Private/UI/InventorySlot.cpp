// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InventorySlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/RaidBossCharacterBase.h"
#include "Components/Image.h"
#include "UI/EquipmentSlot.h"

class URaidBossConsumableItem;

void UInventorySlot::ActivateThisSlot()
{
	TryActivateThisSlotAbility();
}

void UInventorySlot::RegisterNewItem(FGameplayTag InAbilityTriggerTag, int32 Amount, UTexture2D* NewItemImage)
{
	AddDynamicCallBackFunction();
	
	if (Amount > 0)
	{
		AbilityTriggerTag = InAbilityTriggerTag;
		SetItemAmount(Amount);
		SetTexture(NewItemImage);
	}
}

void UInventorySlot::NativeOnInitialized()
{
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	if (InOperation == nullptr)
	{
		return false;
	}
	
	if (UInventorySlot* OtherInventorySlot = Cast<UInventorySlot>(InOperation->Payload))
	{
		SwapInventorySlot(OtherInventorySlot);
	}
	else if (UEquipmentSlot* OtherEquipmentSlot = Cast<UEquipmentSlot>(InOperation->Payload))
	{
		if (OtherEquipmentSlot)
		{
			OtherEquipmentSlot->UnEquipItem();
		}
	}
	
	return true;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (GetItemAmount() > 0)
	{
		BindImage->SetDesiredSizeOverride(FVector2d(BindImage->Brush.ImageSize.X, BindImage->Brush.ImageSize.Y));
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
		OutOperation->Payload = this;
		OutOperation->DefaultDragVisual = BindImage;
		OutOperation->Pivot = DragPivot;
		OutOperation->Offset = DragOffset;
	}
}

FReply UInventorySlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	TryActivateThisSlotAbility();
	
	return UWidgetBlueprintLibrary::Handled().NativeReply;
}

void UInventorySlot::SwapInventorySlot(UInventorySlot* OtherSlot)
{
	if (AbilityTriggerTag.IsValid() == false)
	{
		OtherSlot->RemoveDynamicCallBackFunction();
		this->AddDynamicCallBackFunction();
	}
	
	FGameplayTag		TmpAbilityTriggerTag = OtherSlot->AbilityTriggerTag;
	UTexture2D*			TmpTexture = OtherSlot->GetTexture();
	int32				TmpAmount = OtherSlot->GetItemAmount();
	
	OtherSlot->AbilityTriggerTag = AbilityTriggerTag;
	OtherSlot->SetTexture(GetTexture());
	OtherSlot->SetItemAmount(GetItemAmount());
	
	AbilityTriggerTag = TmpAbilityTriggerTag;
	SetTexture(TmpTexture);
	SetItemAmount(TmpAmount);
}

void UInventorySlot::NotifyItemAmountCallBack(FGameplayTag InAbilityTriggerTag, int32 Amount)
{
	if (InAbilityTriggerTag == AbilityTriggerTag)
	{
		SetItemAmount(Amount);

		if (Amount <= 0)
		{
			ResetSlot();
		}
	}
}

void UInventorySlot::ResetSlot()
{
	RemoveDynamicCallBackFunction();
	AbilityTriggerTag = FGameplayTag{};
	SetItemAmount(0);
	SetTexture(nullptr);
}

void UInventorySlot::AddDynamicCallBackFunction()
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase && AbilityTriggerTag.IsValid() == false)
	{
		CharacterBase->NotifyItemAmountChanged.AddDynamic(this, &UInventorySlot::NotifyItemAmountCallBack);
	}
}

void UInventorySlot::RemoveDynamicCallBackFunction()
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase)
	{
		CharacterBase->NotifyItemAmountChanged.RemoveDynamic(this, &UInventorySlot::NotifyItemAmountCallBack);
	}
}

void UInventorySlot::TryActivateThisSlotAbility() const
{
	ARaidBossCharacterBase*	CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

	if (CharacterBase && AbilityTriggerTag.IsValid())
	{
		FGameplayEventData	EventData;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CharacterBase, AbilityTriggerTag, EventData);
	}
}
