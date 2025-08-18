// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NVAIController.generated.h"

/**
 * 
 */
UCLASS()
class ANVAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANVAIController();

	virtual void OnPossess(APawn* NewPawn) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAISenseConfig_Sight* SightConfig;
};
