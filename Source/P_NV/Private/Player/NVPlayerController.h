// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ANVPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// ���������� ȣ���
	void OnPossess(APawn* NewPawn) override;
	// Ŭ���̾�Ʈ������ ȣ���, ����������.
	void AcknowledgePossession(APawn* NewPawn) override;
private:
	void SpawnGameplayWidget();

	UPROPERTY()
	class ANVPlayerCharacter* NVPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;
};
