// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NVAbilitySystemStatics.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class UNVAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayTag GetBasicAttackAbilityTag();
	static FGameplayTag GetDeadStatTag();

	static float GetStaticCooldownDurationForAbility(const UGameplayAbility* Ability);
	static float GetStaticCostForAbility(const UGameplayAbility* Ability);
};
