#include "Enermy/RaidBossEnermyAnimBase.h"
#include "Enermy/RaidBossEnermyBase.h"
#include "Math/UnrealMathUtility.h"

void URaidBossEnermyAnimBase::PlayDeathMontage()
{
	if (IsValid(DeathMonatage) && Montage_IsPlaying(DeathMonatage) == false)
	{
		Montage_Play(DeathMonatage);

		FOnMontageBlendingOutStarted BlendOutDeathDelegate;

		BlendOutDeathDelegate.BindLambda([this](UAnimMontage*, bool)
			{
				TryGetPawnOwner()->Destroy();
			});
		Montage_SetBlendingOutDelegate(BlendOutDeathDelegate, DeathMonatage);
	}
}

void URaidBossEnermyAnimBase::PlayAttackMontage()
{
	if (IsValid(AttackMonatage) && Montage_IsPlaying(AttackMonatage) == false)
	{
		Montage_Play(AttackMonatage);

		FOnMontageBlendingOutStarted	BlendOutAttackDelegate;

		BlendOutAttackDelegate.BindLambda([this](UAnimMontage*, bool)
			{
				auto AIPawn = Cast<ARaidBossEnermyBase>(TryGetPawnOwner());

				if (IsValid(AIPawn))
					AIPawn->AttackDelegate.Broadcast();
			});
		Montage_SetBlendingOutDelegate(BlendOutAttackDelegate, AttackMonatage);
	}
}
