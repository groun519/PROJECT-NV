// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "DebugShapeHelper.h"
#include "NVGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNVGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	class UAnimInstance* GetOwnerAnimInstance() const;
	
	TArray<FHitResult> GetHitResultFromSweepLocationTargetData(
		const FGameplayAbilityTargetDataHandle& TargetDataHandle,
		float SphereSweepRadius = 30.f,
		ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile,
		bool bDrawDebug = false,
		bool bIgnoreSelf = true) const;
	
	TArray<FHitResult> GetHitResultFromSweepLocationTargetData_Weapon(
		const FGameplayAbilityTargetDataHandle& TargetDataHandle,
		ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile,
		bool bDrawDebug = false, bool bIgnoreSelf = true);

	UFUNCTION()
	FORCEINLINE bool ShouldDrawDebug() const { return bShouldDrawDebug; }
		
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShouldDrawDebug = true;

	// line
	UPROPERTY(Transient)
	FVector PrevBaseLocal = FVector::ZeroVector; // 이전 이벤트의 Base (로컬)

	UPROPERTY(Transient)
	FVector PrevTipLocal  = FVector::ZeroVector; // 이전 이벤트의 Tip  (로컬)
};
