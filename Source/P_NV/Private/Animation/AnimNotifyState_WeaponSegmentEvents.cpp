// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_WeaponSegmentEvents.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameFramework/Character.h"

void UAnimNotifyState_WeaponSegmentEvents::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	// 캐릭터 찾기
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner) return;
	CachedOwner = Owner;
	if (CachedOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Cached ! %s"), *CachedOwner->GetName());
	}
	
	// ASC가 없다면 리턴
	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner)) return;

	// BeginEventTag 이벤트 전송 (ex. Combo01)
	if (bUseBeginEvent && BeginEventTag.IsValid())
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, BeginEventTag, FGameplayEventData());

	// 타이머 설정: 주기 = 세그먼트 길이 = TotalDuration / InterpCount
	const int32 N = FMath::Max(InterpCount, 1);
	if (TotalDuration <= KINDA_SMALL_NUMBER) return;

	UAnimInstance* Anim = MeshComp->GetAnimInstance();
	const UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	if (!Anim || !Montage) return;
	
	const float StepSeconds		= TotalDuration / static_cast<float>(N);
	RemainingSegments			= InterpCount;

	if (UWorld* World = Owner->GetWorld())
	{
		World->GetTimerManager().SetTimer(
			SegmentTimerHandle,
			this, &UAnimNotifyState_WeaponSegmentEvents::OnSegmentTimer,
			/*Rate   =*/ StepSeconds,
			/*bLoop  =*/ true,
			/*First  =*/ 0.0f);
	}
}

void UAnimNotifyState_WeaponSegmentEvents::OnSegmentTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("SegmentTimer()"));

	AActor* Owner = CachedOwner;
	
	// 세그먼트 경계마다 1회 전송
	if (!Owner)
	{
		if (UWorld* World = GWorld)
		{
			World->GetTimerManager().ClearTimer(SegmentTimerHandle);
		}

		UE_LOG(LogTemp, Warning, TEXT("Onwer is nullptr"));
		return;
	}

	(void)SendCurrentLocalSegment();

	--RemainingSegments;
	if (RemainingSegments <= 0)
	{
		if (UWorld* World = CachedOwner->GetWorld())
		{
			World->GetTimerManager().ClearTimer(SegmentTimerHandle);
		}
		UE_LOG(LogTemp, Warning, TEXT("Segment is 0"));
	}
}

void UAnimNotifyState_WeaponSegmentEvents::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 타이머 정리
	if (CachedOwner)
	{
		if (UWorld* World = CachedOwner->GetWorld())
		{
			World->GetTimerManager().ClearTimer(SegmentTimerHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("World is nullptr"));
		}
	}
	RemainingSegments = 0;
	
	if (bUseEndEvent && EndEventTag.IsValid())
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwner, EndEventTag, FGameplayEventData());
}




// Event part

void UAnimNotifyState_WeaponSegmentEvents::SendSegment(const FVector& StartPointLocal, const FVector& EndPointLocal) const
{
	if (!CachedOwner) return;
	if (!AbilityEventTag.IsValid()) return; // AbilityEventTag 미할당 시 리턴
	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CachedOwner)) return;

	FGameplayEventData Data;
	Data.EventTag = AbilityEventTag;
	
	auto* LocInfo = new FGameplayAbilityTargetData_LocationInfo();
	LocInfo->SourceLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->SourceLocation.LiteralTransform.SetLocation(StartPointLocal);
	
	if (TraceType == EVA_Shape::Line)
	{
		LocInfo->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
		LocInfo->TargetLocation.LiteralTransform.SetLocation(EndPointLocal); 
	}

	Data.TargetData = FGameplayAbilityTargetDataHandle(LocInfo);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwner, AbilityEventTag, Data);
}

bool UAnimNotifyState_WeaponSegmentEvents::SendCurrentLocalSegment()
{
	if (!CachedOwner) return false;
	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CachedOwner)) return false;

	ACharacter* Player = Cast<ACharacter>(CachedOwner);
	if (!Player) return false;

	const USkeletalMeshComponent* SkelMesh = Player->GetMesh();
	if (!SkelMesh) return false;

	const FTransform Basis = CachedOwner->GetActorTransform(); // 기준점이 될 액터의 위치 (액터 위치 기반 오프셋을 넘겨줌.)
	
	if (TraceType == EVA_Shape::Line)
	{
		if (!SkelMesh->DoesSocketExist(WeaponBaseSocketName) || !SkelMesh->DoesSocketExist(WeaponTipSocketName))
			return false;

		const FVector BaseLocal = Basis.InverseTransformPosition(SkelMesh->GetSocketLocation(WeaponBaseSocketName));
		const FVector TipLocal  = Basis.InverseTransformPosition(SkelMesh->GetSocketLocation(WeaponTipSocketName));

		SendSegment(BaseLocal, TipLocal);
		return true;
	}
	else
	{
		if (!SkelMesh->DoesSocketExist(TraceSocketName))
			return false;

		const FVector TraceLocal = Basis.InverseTransformPosition(SkelMesh->GetSocketLocation(TraceSocketName));
		SendSegment(TraceLocal);
		return true;
	}
}