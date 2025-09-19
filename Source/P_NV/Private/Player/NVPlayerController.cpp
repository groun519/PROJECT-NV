// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NVPlayerController.h"
#include "Player/NVPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/GameplayWidget.h"

void ANVPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	NVPlayerCharacter = Cast<ANVPlayerCharacter>(NewPawn);
	if (NVPlayerCharacter)
	{
		NVPlayerCharacter->ServerSideInit();
		NVPlayerCharacter->SetGenericTeamId(TeamID);
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

void ANVPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ANVPlayerController::GetGenericTeamId() const
{
	return TeamID;
}

void ANVPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANVPlayerController, TeamID);
}

void ANVPlayerController::SpawnGameplayWidget()
{
	if (!IsLocalPlayerController()) return;

	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
		GameplayWidget->ConfigureAbilities(NVPlayerCharacter->GetAbilities());
	}
}
