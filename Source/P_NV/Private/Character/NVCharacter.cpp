// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NVCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/NVAbilitySystemComponent.h"
#include "GAS/NVAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/OverHeadStatsGauge.h"

ANVCharacter::ANVCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NVAbilitySystemComponent = CreateDefaultSubobject<UNVAbilitySystemComponent>("NVAbility System Component");
	NVAttributeSet = CreateDefaultSubobject<UNVAttributeSet>("NVAttribute Set");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
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

