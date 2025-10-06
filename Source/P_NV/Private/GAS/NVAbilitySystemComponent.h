// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GAS/NVGameplayAbilityTypes.h"
#include "NVAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNVAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UNVAbilitySystemComponent();
	void InitializeBaseAttributes();
	void ServerSideInit();
	void ApplyFullStatEffect();
	const TMap<ENVAbilityInputID, TSubclassOf<UGameplayAbility>>& GetAbilities() const;

private:
	void ApplyInitialEffects();
	void GiveInitialAbilities();
	void AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level = 1);
	void HealthUpdated(const FOnAttributeChangeData& ChangeData);
	void EtherUpdated(const FOnAttributeChangeData& ChangeData);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
	TMap<ENVAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
	TMap<ENVAbilityInputID, TSubclassOf<UGameplayAbility>> BasicAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Base Stats")
	class UPDA_AbilitySystemGenerics* AbilitySystemGenerics;
};
