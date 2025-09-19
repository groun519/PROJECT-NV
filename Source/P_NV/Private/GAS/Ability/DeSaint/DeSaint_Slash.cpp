// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/DeSaint/DeSaint_Slash.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UDeSaint_Slash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayUpperCutMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, UpperCutMontage);
		PlayUpperCutMontageTask->OnBlendOut.AddDynamic(this, &UDeSaint_Slash::K2_EndAbility);
		PlayUpperCutMontageTask->OnCancelled.AddDynamic(this, &UDeSaint_Slash::K2_EndAbility);
		PlayUpperCutMontageTask->OnInterrupted.AddDynamic(this, &UDeSaint_Slash::K2_EndAbility);
		PlayUpperCutMontageTask->OnCompleted.AddDynamic(this, &UDeSaint_Slash::K2_EndAbility);
		PlayUpperCutMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitLaunchEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetSlashLaunchTag());
		WaitLaunchEventTask->EventReceived.AddDynamic(this, &UDeSaint_Slash::StartLaunching);
		WaitLaunchEventTask->ReadyForActivation();
	}
}

FGameplayTag UDeSaint_Slash::GetSlashLaunchTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.DeSaint.Slash.Launch");
}

void UDeSaint_Slash::StartLaunching(FGameplayEventData EventData)
{
	TArray<FHitResult> TargetHitResults =
		GetHitResultFromSweepLocationTargetData_Weapon(
			EventData.TargetData,
			ETeamAttitude::Hostile,
			ShouldDrawDebug(),
			true
		);
	
	if (K2_HasAuthority())
	{
		for (FHitResult& HitResult : TargetHitResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("I Hit: %s"), *HitResult.GetActor()->GetName());
		}
	}
}
