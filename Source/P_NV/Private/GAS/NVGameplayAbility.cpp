// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/NVGameplayAbility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimInstance* UNVGameplayAbility::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMeshComp = GetOwningComponentFromActorInfo();
	if (OwnerSkeletalMeshComp)
	{
		return OwnerSkeletalMeshComp->GetAnimInstance();
	}
	return nullptr;
}

TArray<FHitResult> UNVGameplayAbility::GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, ETeamAttitude::Type TargetTeam, bool bDrawDebug, bool bIgnoreSelf) const
{
	TArray<FHitResult> OutResults;
	TSet<AActor*> HitActors;

	IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());

	for (const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
	{
		FVector StartLoc = TargetData->GetOrigin().GetTranslation();
		FVector EndLoc = TargetData->GetEndPoint();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreSelf)
		{
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		}

		EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> Results;

		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, SphereSweepRadius, ObjectTypes, false, ActorsToIgnore, DrawDebugTrace, Results, false);
	
		for (const FHitResult& Result : Results)
		{
			if (HitActors.Contains(Result.GetActor()))
			{
				continue;
			}

			if (OwnerTeamInterface)
			{
				ETeamAttitude::Type OtherActorTeamAttitude = OwnerTeamInterface->GetTeamAttitudeTowards(*Result.GetActor());
				if (OtherActorTeamAttitude != TargetTeam)
				{
					continue;
				}
			}

			HitActors.Add(Result.GetActor());
			OutResults.Add(Result);
		}
	}

	return OutResults;
}

TArray<FHitResult> UNVGameplayAbility::GetHitResultFromSweepLocationTargetData_Weapon(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	ETeamAttitude::Type TargetTeam,
	bool bDrawDebug, bool bIgnoreSelf)
{
	TArray<FHitResult> OutResults;
	TSet<AActor*> HitActors;

	IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());

	for (const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
	{
		const FVector LocalStart = TargetData->GetOrigin().GetTranslation(); // Base(Local)
		const FVector LocalEnd   = TargetData->GetEndPoint();                // Tip (Local)

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreSelf)
		{
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		}
		
		EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> Results;
		
		// 월드 변환 준비
		const AActor* Avatar   = GetAvatarActorFromActorInfo();
		const FTransform Basis = Avatar ? Avatar->GetActorTransform() : FTransform::Identity;
		const FVector CurBaseW = Basis.TransformPosition(LocalStart);
		const FVector CurTipW  = Basis.TransformPosition(LocalEnd);

		auto DoLine = [&](const FVector& S, const FVector& E)
		{
			if (S.Equals(E)) return; // 0길이 방지
			TArray<FHitResult> Temp;
			UKismetSystemLibrary::LineTraceMultiForObjects(
				this, S, E,
				ObjectTypes, false, ActorsToIgnore,
				DrawDebugTrace, Temp, false,
				FLinearColor::Green, FLinearColor::Red, 1.0f
			);
			Results.Append(Temp);
		};

		if (PrevBaseLocal.IsZero() && PrevTipLocal.IsZero())
		{
			// ★ 첫 프레임: 현재 칼날(Base→Tip)로 한 번은 반드시 쏜다
			DoLine(CurBaseW, CurTipW);

			// 그리고 Prev 갱신
			PrevBaseLocal = LocalStart;
			PrevTipLocal  = LocalEnd;
		}
		else
		{
			// 이전 프레임(로컬) → 월드
			const FVector PrevBaseW = Basis.TransformPosition(PrevBaseLocal);
			const FVector PrevTipW  = Basis.TransformPosition(PrevTipLocal);

			// 누락 방지: 4개 선분 + 현재 프레임 칼날 자체도 1개
			DoLine(PrevBaseW, CurBaseW);
			DoLine(PrevTipW,  CurTipW);
			DoLine(PrevBaseW, CurTipW);

			// Prev 갱신
			PrevBaseLocal = LocalStart;
			PrevTipLocal  = LocalEnd;
		}
		
		for (const FHitResult& Result : Results)
		{
			// 스스로는 피해 x
			if (HitActors.Contains(Result.GetActor()))
			{
				continue;
			}

			/** 대상과의 팀 관계(OtherActorTeamAttitude, PlayerTeam -> TargetTeam)가 TargetTeam과 같지 않으면 피해x
			 * ex1.	TargetTeam				= ETeamAttitude::Friendly	"이 스킬은 아군에게만 적용."
			 *		OtherActorTeamAttitude	= ETeamAttitude::Friendly	"대상 액터의 팀과의 관계는 아군."
			 *			=> 피격 가능 (힐 스킬인데, 적군을 캐스팅했음.)
			 * ex2.	OtherActorTeamAttitude  = ETeamAttitude::Hostile	"이 스킬은 적군에게만 적용."
			 *		TargetTeam				= ETeamAttitude::Friendly	"대상 액터의 팀과의 관계는 아군."
			 *			=> 피격 불가 (딜 스킬인데, 아군이 피격 범위에 존재했음.)
			 */
			if (OwnerTeamInterface)
			{
				ETeamAttitude::Type OtherActorTeamAttitude = OwnerTeamInterface->GetTeamAttitudeTowards(*Result.GetActor());
				if (OtherActorTeamAttitude != TargetTeam)
				{
					continue;
				}
			}

			HitActors.Add(Result.GetActor());
			OutResults.Add(Result);
		}
	}
	return OutResults;
}
