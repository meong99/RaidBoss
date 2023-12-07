#include "Management/RaidBossGameplayTags.h"

RaidBossGameplayTags RaidBossGameplayTags::GameplayTags;

#define TAG(tagName) FGameplayTag::RequestGameplayTag(FName(tagName))

void RaidBossGameplayTags::InitializeTags()
{
	GameplayTags.InitTags();
}

void RaidBossGameplayTags::InitTags()
{
	Attack = TAG("Attack");
	Attack_BasicAttack = TAG("Attack.BasicAttack");
	Attack_RangeAttack = TAG("Attack.RangeAttack");
	
	Event_Weapon_Attack_CollisionEnable = TAG("Event.Weapon.Attack.CollisionEnable");
	Event_Weapon_Attack_CollisionDisable = TAG("Event.Weapon.Attack.CollisionDisable");
	
	Init_Character_MaxHealth = TAG("Init.Character.MaxHealth");
	Init_Character_MaxMana = TAG("Init.Character.MaxMana");
	Init_Character_AttackPower = TAG("Init.Character.AttackPower");
	Init_Character_AttackRange = TAG("Init.Character.AttackRange");
	Init_Character_DefensePower = TAG("Init.Character.DefensePower");
	Init_Character_MoveSpeed = TAG("Init.Character.MoveSpeed");
	Init_Character_AttackSpeed = TAG("Init.Character.AttackSpeed");
	
	Character_Stat_MaxHealth = TAG("Character.Stat.MaxHealth");
	Character_Stat_MaxMana = TAG("Character.Stat.MaxMana");
	Character_Stat_AttackPower = TAG("Character.Stat.AttackPower");
	Character_Stat_AttackRange = TAG("Character.Stat.AttackRange");
	Character_Stat_DefensePower = TAG("Character.Stat.DefensePower");
	Character_Stat_MoveSpeed = TAG("Character.Stat.MoveSpeed");
	Character_Stat_AttackSpeed = TAG("Character.Stat.AttackSpeed");
	
	Character_Additive_MaxHealth = TAG("Character.Additive.MaxHealth");
	Character_Additive_MaxMana = TAG("Character.Additive.MaxMana");
	Character_Additive_AttackPower = TAG("Character.Additive.AttackPower");
	Character_Additive_AttackRange = TAG("Character.Additive.AttackRange");
	Character_Additive_DefensePower = TAG("Character.Additive.DefensePower");
	Character_Additive_MoveSpeed = TAG("Character.Additive.MoveSpeed");
	
	Character_Death = TAG("Character.Death");
	Character_Movement = TAG("Character.Movement");
	
	CoolDown = TAG("CoolDown");

	Animation_Notify_AttackPoint = TAG("Animation.Notify.AttackPoint");
	Animation_Notify_ComboReset = TAG("Animation.Notify.ComboReset");
	Animation_Notify_CanActivateNextAttack = TAG("Animation.Notify.CanActivateNextAttack");
	Animation_Notify_StopMove = TAG("Animation.Notify.StopMove");
	Animation_Notify_BlockMovement = TAG("Animation.Notify.BlockMovement");
	Animation_Notify_ReleaseMovement = TAG("Animation.Notify.ReleaseMovement");
	
	Event_Equipment_ToRemove = TAG("Event.Equipment.ToRemove");

	Event_Skill_IncreaseLevel = TAG("Event.Skill.IncreaseLevel");
	Event_Skill_DecreaseLevel = TAG("Event.Skill.DecreaseLevel");

	StatusEffect_KnockBack = TAG("StatusEffect.Knockback");
	
	Item_SetByCaller_Health = TAG("Item.SetByCaller.Health");
	Item_SetByCaller_MaxHealth = TAG("Item.SetByCaller.MaxHealth");
	Item_SetByCaller_Mana = TAG("Item.SetByCaller.Mana");
	Item_SetByCaller_MaxMana = TAG("Item.SetByCaller.MaxMana");
	Item_SetByCaller_AttackPower = TAG("Item.SetByCaller.AttackPower");
	Item_SetByCaller_DefensePower = TAG("Item.SetByCaller.DefensePower");
	Item_SetByCaller_AdditionalAttackPower = TAG("Item.SetByCaller.AdditionalAttackPower");

	Damage_SetByCaller = TAG("Damage.SetByCaller");
	
	GameplayCue_DestroyActor = TAG("GameplayCue.DestroyActor");
}
