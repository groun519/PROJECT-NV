// Fill out your copyright notice in the Description page of Project Settings.


#include "GAP_Dead.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/NVAbilitySystemStatics.h"

UGAP_Dead::UGAP_Dead()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = UNVAbilitySystemStatics::GetDeadStatTag();

	AbilityTriggers.Add(TriggerData);

	//ActivationBlockedTags.RemoveTag(UNVAbilitySystemStatics::GetStunStatTag());
}

void UGAP_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (K2_HasAuthority())
	{
		AActor* Killer = TriggerEventData->ContextHandle.GetEffectCauser();
		
		if (!Killer || !UNVAbilitySystemStatics::IsPlayer(Killer))
		{
			Killer = nullptr;
		}
		
		bool bFound = false;

		if (Killer)
		{
			UE_LOG(LogTemp, Warning, TEXT("I am Dead, the killer is: %s"), *Killer->GetName());
			
			FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(RewardEffect);
			K2_ApplyGameplayEffectSpecToTarget(EffectSpec, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Killer));
		}
		
		K2_EndAbility();
	}
}