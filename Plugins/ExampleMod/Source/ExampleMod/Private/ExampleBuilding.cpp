// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleBuilding.h"

AExampleBuilding::AExampleBuilding()
{
	// Set building properties
	BuildingName = TEXT("Example Building");
	MaxHealth = 500.0f;
	Health = MaxHealth;
	InfluenceRadius = 750.0f;
	InfluencePerSecond = 2.0f;
}

void AExampleBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("Example Building spawned"));
}

void AExampleBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Example: Do something periodically
	ActionTimer += DeltaTime;
	if (ActionTimer >= 5.0f)
	{
		DoCustomAction();
		ActionTimer = 0.0f;
	}
}

void AExampleBuilding::DoCustomAction()
{
	// Example custom behavior
	UE_LOG(LogTemp, Log, TEXT("Custom action executed! CustomValue: %f"), CustomValue);
	
	// Add your custom building logic here
	// Examples:
	// - Generate resources
	// - Spawn units
	// - Buff nearby units
	// - Heal nearby buildings
}
