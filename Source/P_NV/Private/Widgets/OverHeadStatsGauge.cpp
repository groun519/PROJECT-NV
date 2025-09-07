// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverHeadStatsGauge.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Widgets/ValueGauge.h"
#include "GAS/NVAttributeSet.h"

void UOverHeadStatsGauge::ConfigureWithASC(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HealthBar->DisableText();
		HealthBar->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UNVAttributeSet::GetHealthAttribute(), UNVAttributeSet::GetMaxHealthAttribute());
		EtherBar->DisableText();
		EtherBar->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UNVAttributeSet::GetEtherAttribute(), UNVAttributeSet::GetMaxEtherAttribute());
	}
}
