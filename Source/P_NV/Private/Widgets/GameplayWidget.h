// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/NVGameplayAbilityTypes.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void ConfigureAbilities(const TMap<ENVAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);

private:
	UPROPERTY(meta=(BindWidget))
	class UValueGauge* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* EtherBar;

	UPROPERTY(meta=(BindWidget))
	class UAbilityListView* AbilityListView;

	UPROPERTY()
	class UAbilitySystemComponent* OwnerAbilitySystemComponent;
};
