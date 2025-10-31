// Copyright Epic Games, Inc. All Rights Reserved.

#include "WireframeCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AWireframeCharacter::AWireframeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication
	bReplicates = true;
	SetReplicateMovement(true);

	// Create wireframe mesh component
	WireframeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WireframeMesh"));
	WireframeMesh->SetupAttachment(RootComponent);
	WireframeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Configure character movement
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

void AWireframeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWireframeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update survival stats only on server
	if (HasAuthority())
	{
		UpdateSurvivalStats(DeltaTime);
	}
}

void AWireframeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWireframeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWireframeCharacter, Health);
	DOREPLIFETIME(AWireframeCharacter, Hunger);
	DOREPLIFETIME(AWireframeCharacter, Thirst);
	DOREPLIFETIME(AWireframeCharacter, Stamina);
}

void AWireframeCharacter::UpdateSurvivalStats(float DeltaTime)
{
	// Decrease hunger and thirst over time
	Hunger = FMath::Max(0.0f, Hunger - DeltaTime * 0.5f);
	Thirst = FMath::Max(0.0f, Thirst - DeltaTime * 1.0f);

	// Regenerate stamina when not moving
	if (GetVelocity().Size() < 10.0f)
	{
		Stamina = FMath::Min(100.0f, Stamina + DeltaTime * 10.0f);
	}

	// Take damage if starving or dehydrated
	if (Hunger <= 0.0f || Thirst <= 0.0f)
	{
		TakeDamageCustom(DeltaTime * 5.0f);
	}

	// Death check
	if (Health <= 0.0f)
	{
		// Handle death
		UE_LOG(LogTemp, Warning, TEXT("Character died"));
	}
}

void AWireframeCharacter::TakeDamageCustom(float DamageAmount)
{
	if (!HasAuthority())
		return;

	Health = FMath::Max(0.0f, Health - DamageAmount);
}

void AWireframeCharacter::Heal(float HealAmount)
{
	if (!HasAuthority())
		return;

	Health = FMath::Min(MaxHealth, Health + HealAmount);
}

void AWireframeCharacter::EatFood(float FoodValue)
{
	if (!HasAuthority())
		return;

	Hunger = FMath::Min(100.0f, Hunger + FoodValue);
}

void AWireframeCharacter::DrinkWater(float WaterValue)
{
	if (!HasAuthority())
		return;

	Thirst = FMath::Min(100.0f, Thirst + WaterValue);
}
