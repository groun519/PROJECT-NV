// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "DebugShapeHelper.h"
#include "AnimNotifyState_WeaponSegmentEvents.generated.h"

/**
 * 
 */
UCLASS()
class P_NV_API UAnimNotifyState_WeaponSegmentEvents : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
							 float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	void OnSegmentTimer();
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override;
private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	EVA_Shape TraceType = EVA_Shape::Line;

	/** if Sphere / Box **/
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability", meta=(EditCondition="TraceType!=EVA_Shape::Line"))
	FName TraceSocketName = FName("TraceSocket");

	/** if Line **/
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability", meta=(EditCondition="TraceType==EVA_Shape::Line"))
	FName WeaponBaseSocketName = FName("BaseSocket");

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability", meta=(EditCondition="TraceType==EVA_Shape::Line"))
	FName WeaponTipSocketName = FName("TipSocket");

		////
	
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	FGameplayTag AbilityEventTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	bool bUseBeginEvent = true;
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability", meta=(EditCondition="bUseBeginEvent"))
	FGameplayTag BeginEventTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	bool bUseEndEvent = true;
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability", meta=(EditCondition="bUseEndEvent"))
	FGameplayTag EndEventTag;

	UPROPERTY(EditAnywhere, Category="Interp", meta=(ClampMin="1", AllowPrivateAccess="true"))
	int32 InterpCount = 5;
	
	// Cache
	UPROPERTY()
	TObjectPtr<AActor> CachedOwner;
	
	// Timer
	FTimerHandle SegmentTimerHandle;
	int32 RemainingSegments = 0;
	
	// SendGameplayEvent
	void SendSegment(
		const FVector& Start	= FVector::ZeroVector,
		const FVector& End		= FVector::ZeroVector) const;
	bool SendCurrentLocalSegment();
};
