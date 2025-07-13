// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/NVAttributeSet.h"
#include "Net/UnrealNetwork.h"
/*
* void UNVAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
* {
* 	GAMEPLAYATTRIBUTE_REPNOTIFY(UNVAttributeSet, Health, OldValue)
* }
*
* ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
*
* DEFINE_REPNOTIFY(Health)
*/
#define DEFINE_REPNOTIFY(PropertyName)                              \
    void UNVAttributeSet::OnRep_##PropertyName(                     \
        const FGameplayAttributeData& OldValue)                     \
    {                                                               \
        GAMEPLAYATTRIBUTE_REPNOTIFY(UNVAttributeSet, PropertyName, OldValue); \
    }

void UNVAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UNVAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNVAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNVAttributeSet, Ether, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNVAttributeSet, MaxEther, COND_None, REPNOTIFY_Always);
}

DEFINE_REPNOTIFY(Health)
DEFINE_REPNOTIFY(MaxHealth)
DEFINE_REPNOTIFY(Ether)
DEFINE_REPNOTIFY(MaxEther)