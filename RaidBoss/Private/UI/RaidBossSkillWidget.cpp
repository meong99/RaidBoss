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

void URaidBossSkillWidget::UseSkill(int32 Index)
{
	if (WeakSkillSystem != nullptr)
		WeakSkillSystem->UseSkill(Index);
}

int32 URaidBossSkillWidget::GetSkillLevel(int32 Index)
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->GetSkillLevel(Index);

	return 0;
}

const URaidBossSkillBase* URaidBossSkillWidget::GetSkillInstance(int32 Index) const
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->GetSkillInstance(Index);

	return nullptr;
}

int32 URaidBossSkillWidget::IncreaseSkillLevel(int32 Index)
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->IncreaseSkillLevel(Index);

	return 0;
}

int32 URaidBossSkillWidget::DecreaseSkillLevel(int32 Index)
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->DecreaseSkillLevel(Index);

	return 0;
}

void URaidBossSkillWidget::CreateSlots()
{
	if (WeakSkillSystem != nullptr)
	{
		int32 SkillCount = WeakSkillSystem->GetSkillCount();
		
		for (int i = 0; i < SkillCount; i++)
		{
			const URaidBossSkillBase*	Skill = WeakSkillSystem->GetSkillCDO(i);
			URaidBossSlotWidget*		SkillSlot = CreateNewSlot();
			
			SkillSlot->SetSlotType(ESlotType::SkillSlot);
			SkillSlot->SetIndex(i);
			SkillSlot->SetTexture(Skill->GetSkillInfo().SkillTexture);
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

const URaidBossSkillBase* URaidBossSkillWidget::GetSkillCDO(int32 Index) const
{
	if (WeakSkillSystem != nullptr)
		return WeakSkillSystem->GetSkillCDO(Index);
	
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
