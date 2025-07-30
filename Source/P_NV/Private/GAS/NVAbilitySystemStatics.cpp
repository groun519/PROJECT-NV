// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/NVAbilitySystemStatics.h"

FGameplayTag UNVAbilitySystemStatics::GetBasicAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.BasicAttack");
}
