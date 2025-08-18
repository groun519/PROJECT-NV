// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "NVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ANVPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	// 서버에서만 호출됨
	void OnPossess(APawn* NewPawn) override;
	// 클라이언트에서만 호출됨, 리슨서버도.
	void AcknowledgePossession(APawn* NewPawn) override;
	// Assigns Team Agent to given TeamID
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	// Retrieve team identifier in form of FGenericTeamId
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:
	void SpawnGameplayWidget();

	UPROPERTY()
	class ANVPlayerCharacter* NVPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;

	UPROPERTY(Replicated)
	FGenericTeamId TeamID;
};
