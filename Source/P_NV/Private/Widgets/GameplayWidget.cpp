// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/NVAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Widgets/AbilityListView.h"
#include "Widgets/ValueGauge.h"
#include "GAS/NVAttributeSet.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerAbilitySystemComponent)
	{
		HealthBar->SetAndBoundToGameplayAttribute(OwnerAbilitySystemComponent, UNVAttributeSet::GetHealthAttribute(), UNVAttributeSet::GetMaxHealthAttribute());
		EtherBar->SetAndBoundToGameplayAttribute(OwnerAbilitySystemComponent, UNVAttributeSet::GetHealthAttribute(), UNVAttributeSet::GetMaxHealthAttribute());
	}
}

void UGameplayWidget::ConfigureAbilities(const TMap<ENVAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities)
{
	AbilityListView->ConfigureAbilities(Abilities);
}
