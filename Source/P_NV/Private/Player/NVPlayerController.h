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
	UPROPERTY()
	class ANVPlayerCharacter* NVPlayerCharacter;
};
