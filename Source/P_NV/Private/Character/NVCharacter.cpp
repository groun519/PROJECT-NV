// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NVCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/NVAbilitySystemComponent.h"
#include "GAS/NVAttributeSet.h"
#include "GAS/NVAbilitySystemStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/OverHeadStatsGauge.h"

ANVCharacter::ANVCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NVAbilitySystemComponent = CreateDefaultSubobject<UNVAbilitySystemComponent>("NVAbility System Component");
	NVAttributeSet = CreateDefaultSubobject<UNVAttributeSet>("NVAttribute Set");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());

	BindGASChangeDelegates();
}

void ANVCharacter::ServerSideInit()
{
	NVAbilitySystemComponent->InitAbilityActorInfo(this, this);
	NVAbilitySystemComponent->ApplyInitialEffects();
	NVAbilitySystemComponent->GiveInitialAbilities();
}

void ANVCharacter::ClientSideInit()
{
	NVAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

bool ANVCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ANVCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANVCharacter, TeamID);
}

void ANVCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

void ANVCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ConfigureOverHeadStatsWidget();
}

void ANVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANVCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ANVCharacter::GetAbilitySystemComponent() const
{
	return NVAbilitySystemComponent;
}

void ANVCharacter::BindGASChangeDelegates()
{
	if (NVAbilitySystemComponent)
	{
		NVAbilitySystemComponent->RegisterGameplayTagEvent(UNVAbilitySystemStatics::GetDeadStatTag()).AddUObject(this, &ANVCharacter::DeathTagUpdated);
	}
}

void ANVCharacter::DeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
	{
		StartDeathSequence();
	}
	else
	{
		Respawn();
	}
}

void ANVCharacter::ConfigureOverHeadStatsWidget()
{
	if (!OverHeadWidgetComponent) return;

	if (IsLocallyControlledByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}

	UOverHeadStatsGauge* OverheadStatsGauge = Cast<UOverHeadStatsGauge>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (OverheadStatsGauge)
	{
		OverheadStatsGauge->ConfigureWithASC(GetAbilitySystemComponent());
		OverHeadWidgetComponent->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(HeadStatGaugeVisibilityUpdateTimerHandle, this, &ANVCharacter::UpdateHeadGaugeVisibility, HeadStatGaugeVisibilityCheckUpdateGap, true);
	}
}

void ANVCharacter::UpdateHeadGaugeVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverHeadWidgetComponent->SetHiddenInGame(DistSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}

void ANVCharacter::SetStatusGaugeEnabled(bool bIsEnabled)
{
	GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);
	if (bIsEnabled)
	{
		ConfigureOverHeadStatsWidget();
	}
	else
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
	}
}

void ANVCharacter::DeathMontageFinished()
{
	SetRagdollEnabled(true);
}

void ANVCharacter::SetRagdollEnabled(bool bIsEnabled)
{
	if (bIsEnabled)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshRelativeTransform);
	}
}

void ANVCharacter::PlayDeathAnimation()
{
	if (DeathMontage)
	{
		float MontageDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(DeathMontageTimerHandle, this, &ANVCharacter::DeathMontageFinished, MontageDuration + DeathMontageFinishTimeShift);
	}
}

void ANVCharacter::StartDeathSequence()
{
	OnDead();
	PlayDeathAnimation();
	SetStatusGaugeEnabled(false);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANVCharacter::Respawn()
{
	OnRespawn();
	SetRagdollEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);

	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}

	if (NVAbilitySystemComponent)
	{
		NVAbilitySystemComponent->ApplyFullStatEffect();
	}
}

void ANVCharacter::OnDead()
{
}

void ANVCharacter::OnRespawn()
{
	OnRespawn();
	SetRagdollEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);

	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}

	if (NVAbilitySystemComponent)
	{
		NVAbilitySystemComponent->ApplyFullStatEffect();
	}
}

void ANVCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ANVCharacter::GetGenericTeamId() const
{
	return TeamID;
}

