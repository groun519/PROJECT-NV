// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "NVCharacter.generated.h"

UCLASS()
class ANVCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANVCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Gameplay Ability **/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:
	UPROPERTY()
	class UNVAbilitySystemComponent* NVAbilitySystemComponent;
	UPROPERTY()
	class UNVAttributeSet* NVAttributeSet;
};
