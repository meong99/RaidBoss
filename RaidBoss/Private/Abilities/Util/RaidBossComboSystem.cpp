﻿#include "Abilities/Util/RaidBossComboSystem.h"

URaidBossComboSystem::URaidBossComboSystem()
{
}

void URaidBossComboSystem::Tick(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= ResetTime)
		ResetStack();
}

uint8 URaidBossComboSystem::GetCurrentStack() const
{
	return Stack;
}

void URaidBossComboSystem::SetMaxStack(uint8 Value)
{
	MaxStack = Value;
}

void URaidBossComboSystem::IncreaseStack()
{
	ElapsedTime = 0;
	Stack++;
	if (Stack >= MaxStack)
		ResetStack();
}

void URaidBossComboSystem::ResetStack()
{
	SetIsTickable(false);
	ElapsedTime = 0;
	Stack = 0;
}

void URaidBossComboSystem::SetResetTime(float Time)
{
	ResetTime = Time;
}
