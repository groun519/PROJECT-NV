// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NVAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNVAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void ApplyInitialEffects();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
};
