// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/NVGameplayAbility.h"
#include "DeSaint_Slash.generated.h"

/**
 * 
 */
UCLASS()
class UDeSaint_Slash : public UNVGameplayAbility
{
	GENERATED_BODY()
public:	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* UpperCutMontage;

	static FGameplayTag GetSlashLaunchTag();

	UFUNCTION()
	void StartLaunching(FGameplayEventData EventData);
};
