// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/NVAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GAS/NVGameplayAbilityTypes.h"
#include "GAS/NVAttributeSet.h"
#include "GAS/NVAbilitySystemStatics.h"
#include "GAS/Data/PDA_AbilitySystemGenerics.h"

UNVAbilitySystemComponent::UNVAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UNVAttributeSet::GetHealthAttribute()).AddUObject(this, &UNVAbilitySystemComponent::HealthUpdated);
	GetGameplayAttributeValueChangeDelegate(UNVAttributeSet::GetEtherAttribute()).AddUObject(this, &UNVAbilitySystemComponent::EtherUpdated);
}

void UNVAbilitySystemComponent::InitializeBaseAttributes()
{
	if (!AbilitySystemGenerics || ! AbilitySystemGenerics->GetBaseStatDataTable() || !GetOwner())
	{
		return;
	}

	const UDataTable* BaseStatDataTable = AbilitySystemGenerics->GetBaseStatDataTable();
	const FPlayerBaseStats* BaseStats = nullptr;

	for (const TPair<FName, uint8*>& DataPair : BaseStatDataTable->GetRowMap())
	{
		BaseStats = BaseStatDataTable->FindRow<FPlayerBaseStats>(DataPair.Key, "");
		if (BaseStats && BaseStats->Class == GetOwner()->GetClass())
		{
			break;
		}
	}

	if (BaseStats)
	{
		SetNumericAttributeBase(UNVAttributeSet::GetMaxHealthAttribute(), BaseStats->BaseMaxHealth);
		SetNumericAttributeBase(UNVAttributeSet::GetMaxEtherAttribute(), BaseStats->BaseMaxEther);
		SetNumericAttributeBase(UNVAttributeSet::GetAttackDamageAttribute(), BaseStats->BaseAttackDamage);
		SetNumericAttributeBase(UNVAttributeSet::GetArmorAttribute(), BaseStats->BaseArmor);
		SetNumericAttributeBase(UNVAttributeSet::GetMoveSpeedAttribute(), BaseStats->BaseMoveSpeed);
	}
}

void UNVAbilitySystemComponent::ServerSideInit()
{
	InitializeBaseAttributes();
	ApplyInitialEffects();
	GiveInitialAbilities();
}

void UNVAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (!AbilitySystemGenerics) return;
	
	for (const TSubclassOf<UGameplayEffect>& EffectClass : AbilitySystemGenerics->GetInitialEffects())
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UNVAbilitySystemComponent::GiveInitialAbilities()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	for (const TPair<ENVAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 0, (int32)AbilityPair.Key, nullptr));
	}

	for (const TPair<ENVAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, (int32)AbilityPair.Key, nullptr));
	}

	if (!AbilitySystemGenerics) return;
	
	for (const TSubclassOf<UGameplayAbility>& PassiveAbility : AbilitySystemGenerics->GetPassiveAbilities())
	{
		GiveAbility(FGameplayAbilitySpec(PassiveAbility, 1, -1, nullptr));
	}
}

void UNVAbilitySystemComponent::ApplyFullStatEffect()
{
	if (!AbilitySystemGenerics) return;
	AuthApplyGameplayEffect(AbilitySystemGenerics->GetFullStatEffect());
}

const TMap<ENVAbilityInputID, TSubclassOf<UGameplayAbility>>& UNVAbilitySystemComponent::GetAbilities() const
{
	return Abilities;
}

void UNVAbilitySystemComponent::AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffect, Level, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UNVAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	bool bFound = false;
	float MaxHealth = GetGameplayAttributeValue(UNVAttributeSet::GetMaxHealthAttribute(), bFound);
	if (bFound && ChangeData.NewValue >= MaxHealth)
	{
		if (!HasMatchingGameplayTag(UNVAbilitySystemStatics::GetHealthFullStatTag()))
		{
			//This is done local only.
			AddLooseGameplayTag(UNVAbilitySystemStatics::GetHealthFullStatTag());
		}
	}
	else
	{
		RemoveLooseGameplayTag(UNVAbilitySystemStatics::GetHealthFullStatTag());
	}

	if (ChangeData.NewValue <= 0)
	{
		if (!HasMatchingGameplayTag(UNVAbilitySystemStatics::GetHealthEmptyStatTag()))
		{
			AddLooseGameplayTag(UNVAbilitySystemStatics::GetHealthEmptyStatTag());

			if(AbilitySystemGenerics && AbilitySystemGenerics->GetDeathEffect())
				AuthApplyGameplayEffect(AbilitySystemGenerics->GetDeathEffect());

			FGameplayEventData DeadAbilityEventData;
			if(ChangeData.GEModData)
				DeadAbilityEventData.ContextHandle = ChangeData.GEModData->EffectSpec.GetContext();

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), UNVAbilitySystemStatics::GetDeadStatTag(), DeadAbilityEventData);
		}
	}
	else
	{
		RemoveLooseGameplayTag(UNVAbilitySystemStatics::GetHealthEmptyStatTag());
	}
}

void UNVAbilitySystemComponent::EtherUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	bool bFound = false;
	float MaxMana = GetGameplayAttributeValue(UNVAttributeSet::GetMaxEtherAttribute(), bFound);
	if (bFound && ChangeData.NewValue >= MaxMana)
	{
		if (!HasMatchingGameplayTag(UNVAbilitySystemStatics::GetEtherFullStatTag()))
		{
			//This is done local only.
			AddLooseGameplayTag(UNVAbilitySystemStatics::GetEtherFullStatTag());
		}
	}
	else
	{
		RemoveLooseGameplayTag(UNVAbilitySystemStatics::GetEtherFullStatTag());
	}

	if (ChangeData.NewValue <= 0)
	{
		if (!HasMatchingGameplayTag(UNVAbilitySystemStatics::GetEtherEmptyStatTag()))
		{
			AddLooseGameplayTag(UNVAbilitySystemStatics::GetEtherEmptyStatTag());
		}
	}
	else
	{
		RemoveLooseGameplayTag(UNVAbilitySystemStatics::GetEtherEmptyStatTag());
	}
}
