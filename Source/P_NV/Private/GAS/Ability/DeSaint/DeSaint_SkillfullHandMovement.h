// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/NVGameplayAbility.h"
#include "DeSaint_SkillfullHandMovement.generated.h"

/**
 * 
 */
UCLASS()
class P_NV_API UDeSaint_SkillfullHandMovement : public UNVGameplayAbility
{
	GENERATED_BODY()

public:	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> SkillfullHandMovementDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* SkillfullHandMovementMontage;

	static FGameplayTag GetSkillfullHandMovementHitTag();

	UFUNCTION()
	void StartHit(FGameplayEventData EventData);
};
