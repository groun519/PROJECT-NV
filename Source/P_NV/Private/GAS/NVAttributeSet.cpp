// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/NVAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
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
DEFINE_REPNOTIFY(AttackDamage)
DEFINE_REPNOTIFY(Armor)
DEFINE_REPNOTIFY(MoveSpeed)

void UNVAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetEtherAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEther());
	}
}

void UNVAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		SetCachedHealthPercent(GetHealth()/GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetEtherAttribute())
	{
		SetEther(FMath::Clamp(GetEther(), 0, GetMaxEther()));
		SetCachedEtherPercent(GetEther()/GetMaxEther());
	}
}

void UNVAttributeSet::RescaleHealth()
{
	if (!GetOwningActor()->HasAuthority())
		return;

	if (GetCachedHealthPercent() != 0 && GetHealth() != 0)
	{
		SetHealth(GetMaxHealth() * GetCachedHealthPercent());
	}
}

void UNVAttributeSet::RescaleMana()
{
	if (!GetOwningActor()->HasAuthority())
		return;

	if (GetCachedEtherPercent() != 0 && GetEther() != 0)
	{
		SetEther(GetMaxEther() * GetCachedEtherPercent());
	}
}
