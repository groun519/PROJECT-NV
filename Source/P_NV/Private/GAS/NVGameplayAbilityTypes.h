// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "NVGameplayAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class ENVAbilityInputID : uint8
{
	None				UMETA(DisplayName = "None"),

	BasicAttack			UMETA(DisplayName = "Basic Attack"),
	Ability_Passive		UMETA(DisplayName = "Ability Passive"),
	Ability_Q			UMETA(DisplayName = "Ability Q"),
	Ability_E			UMETA(DisplayName = "Ability E"),
	Ability_R			UMETA(DisplayName = "Ability R"),
	Ability_Ultimate	UMETA(DisplayName = "Ability Ultimate"),
	Ability_Block		UMETA(DisplayName = "Ability Block"),
	Ability_Evasion		UMETA(DisplayName = "Ability Evasion"),

	Confirm				UMETA(DisplayName = "Confirm"),
	Cancel				UMETA(DisplayName = "Cancel"),
};

USTRUCT(BlueprintType)
struct FGenericDamgeEffectDef
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

USTRUCT(BlueprintType)
struct FPlayerBaseStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Class;

	UPROPERTY(EditAnywhere)
	float BaseMaxHealth;

	UPROPERTY(EditAnywhere)
	float BaseMaxEther;

	UPROPERTY(EditAnywhere)
	float BaseAttackDamage;

	UPROPERTY(EditAnywhere)
	float BaseArmor;

	UPROPERTY(EditAnywhere)
	float BaseMoveSpeed;
};