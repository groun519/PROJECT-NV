// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NVPlayerController.h"
#include "Player/NVPlayerCharacter.h"
#include "Widgets/GameplayWidget.h"

void ANVPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	NVPlayerCharacter = Cast<ANVPlayerCharacter>(NewPawn);
	if (NVPlayerCharacter)
	{
		NVPlayerCharacter->ServerSideInit();
	}
}

void ANVPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	NVPlayerCharacter = Cast<ANVPlayerCharacter>(NewPawn);
	if (NVPlayerCharacter)
	{
		NVPlayerCharacter->ClientSideInit();
		SpawnGameplayWidget();
	}
}

void ANVPlayerController::SpawnGameplayWidget()
{
	if (!IsLocalPlayerController()) return;

	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}
