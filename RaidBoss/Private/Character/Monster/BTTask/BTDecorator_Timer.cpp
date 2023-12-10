// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Monster/BTTask/BTDecorator_Timer.h"

void UBTDecorator_Timer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("asd"));
}

bool UBTDecorator_Timer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
