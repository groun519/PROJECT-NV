// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NVAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UNVAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, Ether)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, MaxEther)

private:
	UPROPERTY()
	FGameplayAttributeData Health;
	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	UPROPERTY()
	FGameplayAttributeData Ether;
	UPROPERTY()
	FGameplayAttributeData MaxEther;
	
};
