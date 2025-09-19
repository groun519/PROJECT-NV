// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "GAS/NVGameplayAbilityTypes.h"
#include "AbilityListView.generated.h"

/**
 * 
 */
UCLASS()
class P_NV_API UAbilityListView : public UListView
{
	GENERATED_BODY()
	
public:
	void ConfigureAbilities(const TMap<ENVAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* AbilityDataTable;

	void AbilityGaugeGenerated(UUserWidget& Widget);

	const struct FAbilityWidgetData* FindWidgetDataForAbility(const TSubclassOf<UGameplayAbility>& AbilityClass) const;
};
