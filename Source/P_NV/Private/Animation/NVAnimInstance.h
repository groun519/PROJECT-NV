// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNVAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintTreadSafe))
	FORCEINLINE float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintTreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed != 0; }
	UFUNCTION(BlueprintCallable, meta = (BlueprintTreadSafe))
	FORCEINLINE bool IsNotMoving() const { return Speed == 0; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintTreadSafe))
	FORCEINLINE bool GetIsJumping() const { return bIsJumping; }
	UFUNCTION(BlueprintCallable, meta = (BlueprintTreadSafe))
	FORCEINLINE bool GetIsOnGround() const { return !bIsJumping; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintTreadSafe))
	FORCEINLINE float GetLookYawOffset() const { return LookRotOffset.Yaw; }
	UFUNCTION(BlueprintCallable, meta = (BlueprintTreadSafe))
	FORCEINLINE float GetLookPitchOffset() const { return LookRotOffset.Pitch; }

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* OwnerMovementComp;

	float Speed;
	bool bIsJumping;

	FRotator LookRotOffset;
};
