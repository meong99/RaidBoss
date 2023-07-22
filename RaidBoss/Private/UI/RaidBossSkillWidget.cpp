#include "UI/RaidBossSkillWidget.h"
#include "Abilities/Skill/RaidBossSkillBase.h"
#include "Components/ScrollBox.h"
#include "Management/RaidBossSkillSystem.h"
#include "UI/RaidBossSlotWidget.h"

void URaidBossSkillWidget::InitializeSkillWidget(URaidBossSkillSystem* InWeakSkillSystem)
{
	WeakSkillSystem = InWeakSkillSystem;
	CreateSlots();
}

void URaidBossSkillWidget::CreateSlots()
{
	URaidBossSlotWidget*	SkillSlot;
	URaidBossSkillBase*		Skill;
	
	if (WeakSkillSystem != nullptr)
	{
		auto Classes = WeakSkillSystem->GetSkillClasses();
		
		for (int i = 0; i < Classes.Num(); i++)
		{
			Skill = Cast<URaidBossSkillBase>(Classes[i]->GetDefaultObject());
			
			SkillSlot = CreateNewSlot();
			SkillSlot->SetIndex(i);
			SkillSlot->SetTexture(Skill->GetSkillInfo().SkillTexture);
			SkillSlot->SetSlotType(ESlotType::SkillSlot);
			SkillSlot->SetWeakOwnerWidget(this);
			ScrollBox->AddChild(SkillSlot);
			SkillSlots.Add(SkillSlot);
		}
	}
}

URaidBossSlotWidget* URaidBossSkillWidget::CreateNewSlot()
{
	URaidBossSlotWidget* SkillSlot = Cast<URaidBossSlotWidget>(CreateWidget(this, SlotClass));

	return SkillSlot;
}

const URaidBossSkillBase* URaidBossSkillWidget::GetSkillObject(int32 Index) const
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->GetSkillObject(Index);
	
	return nullptr;
}

int32 URaidBossSkillWidget::GetCurrentSkillPoint() const
{
	if (WeakSkillSystem != nullptr)
	{
		return WeakSkillSystem->GetCurrentSkillPoint();
	}

	return 0;
}
