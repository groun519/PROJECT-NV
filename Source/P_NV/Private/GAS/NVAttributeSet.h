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

#define DECLARE_REPNOTIFY(PropertyName)                           \
	UPROPERTY(ReplicatedUsing = OnRep_##PropertyName) FGameplayAttributeData PropertyName; \
    UFUNCTION() void OnRep_##PropertyName(const FGameplayAttributeData& OldValue);

/**
 * 
 */
UCLASS()
class UNVAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, Ether)
	ATTRIBUTE_ACCESSORS(UNVAttributeSet, MaxEther)
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_Health)		FGameplayAttributeData Health;
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)	FGameplayAttributeData MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_Ether)		FGameplayAttributeData Ether;
	UPROPERTY(ReplicatedUsing = OnRep_MaxEther)		FGameplayAttributeData MaxEther;
	
	UFUNCTION()	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Ether(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MaxEther(const FGameplayAttributeData& OldValue);
}; 