// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuickSlotUI.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Character/RaidBossCharacterBase.h"
#include "UI/QuickSlot.h"

void UQuickSlotUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ARaidBossCharacterBase* CharacterBase = Cast<ARaidBossCharacterBase>(GetOwningPlayerPawn());

    if (CharacterBase)
    {
        CharacterBase->NotifyNewWeaponEquipped.AddDynamic(this, &UQuickSlotUI::NotifyNewWeaponEquippedCallBack);
    }
}

void UQuickSlotUI::NotifyNewWeaponEquippedCallBack(const TArray<URaidBossSkillBase*>& NewSkills)
{
    RegisterNewSKills(NewSkills);
}

void UQuickSlotUI::ResetAllSkillSlots()
{
    QuickSlot_Q->RemoveRegisteredSkill();
    QuickSlot_E->RemoveRegisteredSkill();
    QuickSlot_R->RemoveRegisteredSkill();
    QuickSlot_Right->RemoveRegisteredSkill();
}

void UQuickSlotUI::RegisterNewSKills(const TArray<URaidBossSkillBase*>& NewSkills)
{
    if (NewSkills.IsEmpty())
    {
        return;
    }

    for (int i = 0; i < NewSkills.Num(); i++)
    {
        URaidBossSkillBase* NewSkill = NewSkills[i];

        if (NewSkill == nullptr)
        {
            continue;
        }

        switch (i)
        {
        case 0:
            QuickSlot_Q->SetNewSkill(NewSkill);
            break;
        case 1:
            QuickSlot_E->SetNewSkill(NewSkill);
            break;
        case 2:
            QuickSlot_R->SetNewSkill(NewSkill);
            break;
        case 3:
            QuickSlot_Right->SetNewSkill(NewSkill);
            break;
        default:
            break;
        }
    }
}
