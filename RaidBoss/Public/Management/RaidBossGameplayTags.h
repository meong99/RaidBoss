#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct RaidBossGameplayTags
{
private:
	static RaidBossGameplayTags GameplayTags;
public:
	FGameplayTag Attack;
	FGameplayTag Attack_BasicAttack;
	FGameplayTag Attack_RangeAttack;
	
	FGameplayTag Init_Character_MaxHealth;
	FGameplayTag Init_Character_MaxMana;
	FGameplayTag Init_Character_AttackPower;
	FGameplayTag Init_Character_AttackRange;
	FGameplayTag Init_Character_DefensePower;
	FGameplayTag Init_Character_MoveSpeed;
	FGameplayTag Init_Character_AttackSpeed;
	
	FGameplayTag Character_Additive_MaxHealth;
	FGameplayTag Character_Additive_MaxMana;
	FGameplayTag Character_Additive_AttackPower;
	FGameplayTag Character_Additive_AttackRange;
	FGameplayTag Character_Additive_DefensePower;
	FGameplayTag Character_Additive_MoveSpeed;
	FGameplayTag Character_Death;
	FGameplayTag Character_Movement;
	
	FGameplayTag Event_Weapon_Attack_CollisionEnable;
	FGameplayTag Event_Weapon_Attack_CollisionDisable;
	
	FGameplayTag CoolDown;

	FGameplayTag Animation_Notify_AttackPoint;
	FGameplayTag Animation_Notify_ComboReset;
	FGameplayTag Animation_Notify_CanActivateNextAttack;
	FGameplayTag Animation_Notify_StopMove;
	FGameplayTag Animation_Notify_BlockMovement;
	FGameplayTag Animation_Notify_ReleaseMovement;

	
	FGameplayTag Event_Equipment_ToRemove;
	FGameplayTag Event_Skill_IncreaseLevel;
	FGameplayTag Event_Skill_DecreaseLevel;

	FGameplayTag StatusEffect_KnockBack;

	FGameplayTag Item_SetByCaller_Health;
	FGameplayTag Item_SetByCaller_MaxHealth;
	FGameplayTag Item_SetByCaller_Mana;
	FGameplayTag Item_SetByCaller_MaxMana;
	FGameplayTag Item_SetByCaller_AttackPower;
	FGameplayTag Item_SetByCaller_DefensePower;
	FGameplayTag Item_SetByCaller_AdditionalAttackPower;
public:
	static void InitializeTags();
	static const RaidBossGameplayTags& Get() { return GameplayTags; }
	
private:
	void InitTags();
};
