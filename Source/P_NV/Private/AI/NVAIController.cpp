// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/NVAIController.h"
#include "Character/NVCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ANVAIController::ANVAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	
	SightConfig->SetMaxAge(5.f);

	SightConfig->PeripheralVisionAngleDegrees = 180.f;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void ANVAIController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	SetGenericTeamId(FGenericTeamId(0));

	IGenericTeamAgentInterface* PawnTeamInterface = Cast<IGenericTeamAgentInterface>(NewPawn);
	if (PawnTeamInterface)
	{
		PawnTeamInterface->SetGenericTeamId(GetGenericTeamId());
	}
}
