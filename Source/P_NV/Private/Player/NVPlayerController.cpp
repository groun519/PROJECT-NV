// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NVPlayerController.h"
#include "Player/NVPlayerCharacter.h"

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
	}
}
