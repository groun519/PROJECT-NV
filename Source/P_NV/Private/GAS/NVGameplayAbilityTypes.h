// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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