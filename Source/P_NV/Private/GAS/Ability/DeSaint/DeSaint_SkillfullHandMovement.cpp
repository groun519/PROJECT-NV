// Fill out your copyright notice in the Description page of Project Settings.


#include "DeSaint_SkillfullHandMovement.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UDeSaint_SkillfullHandMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayUpperCutMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillfullHandMovementMontage);
		PlayUpperCutMontageTask->OnBlendOut.AddDynamic(this, &UDeSaint_SkillfullHandMovement::K2_EndAbility);
		PlayUpperCutMontageTask->OnCancelled.AddDynamic(this, &UDeSaint_SkillfullHandMovement::K2_EndAbility);
		PlayUpperCutMontageTask->OnInterrupted.AddDynamic(this, &UDeSaint_SkillfullHandMovement::K2_EndAbility);
		PlayUpperCutMontageTask->OnCompleted.AddDynamic(this, &UDeSaint_SkillfullHandMovement::K2_EndAbility);
		PlayUpperCutMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitLaunchEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetSkillfullHandMovementHitTag());
		WaitLaunchEventTask->EventReceived.AddDynamic(this, &UDeSaint_SkillfullHandMovement::StartHit);
		WaitLaunchEventTask->ReadyForActivation();
	}
}

FGameplayTag UDeSaint_SkillfullHandMovement::GetSkillfullHandMovementHitTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.DeSaint.SkillfullHandMovement.Damage");
}

void UDeSaint_SkillfullHandMovement::StartHit(FGameplayEventData EventData)
{
	if (K2_HasAuthority())
	{
		TArray<FHitResult> TargetHitResults = GetHitResultFromSweepLocationTargetData_Weapon(EventData.TargetData, ETeamAttitude::Hostile, ShouldDrawDebug(), true);
		for (FHitResult& HitResult : TargetHitResults)
		{
			ApplyGameplayEffectToHitResultActor(HitResult, SkillfullHandMovementDamageEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		}
	}
}
