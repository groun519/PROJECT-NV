// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "NVCharacter.generated.h"

UCLASS()
class ANVCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ANVCharacter();
	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControlledByPlayer() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	const TMap<ENVAbilityInputID, TSubclassOf<UGameplayAbility>>& GetAbilities() const;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Gameplay Ability **/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	void BindGASChangeDelegates();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	class UNVAbilitySystemComponent* NVAbilitySystemComponent;
	UPROPERTY()
	class UNVAttributeSet* NVAttributeSet;

	/** UI **/
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* OverHeadWidgetComponent;
	void ConfigureOverHeadStatsWidget();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 1000000.f;

	FTimerHandle HeadStatGaugeVisibilityUpdateTimerHandle;

	void UpdateHeadGaugeVisibility();
	void SetStatusGaugeEnabled(bool bIsEnabled);

	/** Death and Respawn **/
public:
	bool IsDead() const;
	void RespawnImmediately();
	
private:
	FTransform MeshRelativeTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathMontageFinishTimeShift = -0.8f;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;
	
	FTimerHandle DeathMontageTimerHandle;

	void DeathMontageFinished();
	void SetRagdollEnabled(bool bIsEnabled);

	void PlayDeathAnimation();

	void StartDeathSequence();
	void Respawn();

	virtual void OnDead();
	virtual void OnRespawn();

	/** Team **/
public:
	// Assigns Team Agent to given TeamID
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	// Retrieve team identifier in form of FGenericTeamId
	virtual FGenericTeamId GetGenericTeamId() const override;
private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamID)
	FGenericTeamId TeamID;

	UFUNCTION()
	virtual void OnRep_TeamID();
};
