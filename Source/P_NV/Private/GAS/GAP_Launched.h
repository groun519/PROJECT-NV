// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NVGameplayAbility.h"
#include "GAP_Launched.generated.h"

/**
 * 
 */
UCLASS()
class P_NV_API UGAP_Launched : public UNVGameplayAbility
{
	GENERATED_BODY()

public:
	UGAP_Launched();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
