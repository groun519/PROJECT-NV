// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NVCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GAS/NVAbilitySystemComponent.h"
#include "GAS/NVAttributeSet.h"

ANVCharacter::ANVCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NVAbilitySystemComponent = CreateDefaultSubobject<UNVAbilitySystemComponent>("NVAbility System Component");
	NVAttributeSet = CreateDefaultSubobject<UNVAttributeSet>("NVAttribute Set");
}

void ANVCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

