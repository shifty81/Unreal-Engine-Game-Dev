// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hangar.h"
#include "Net/UnrealNetwork.h"

AHangar::AHangar()
{
	BuildingName = TEXT("Hangar");
	
	// Hangars provide more influence
	InfluencePerSecond = 5.0f;
	InfluenceRadius = 1000.0f;
}

void AHangar::BeginPlay()
{
	Super::BeginPlay();
}

void AHangar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update ship construction
	if (HasAuthority() && !CurrentShipDesign.IsEmpty() && ConstructionProgress < 1.0f)
	{
		ConstructionProgress += ConstructionSpeed * DeltaTime;
		
		if (ConstructionProgress >= 1.0f)
		{
			ConstructionProgress = 1.0f;
			UE_LOG(LogTemp, Log, TEXT("Ship construction complete!"));
		}
	}
}

void AHangar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHangar, StoredShipDesigns);
	DOREPLIFETIME(AHangar, CurrentShip);
	DOREPLIFETIME(AHangar, ConstructionProgress);
}

void AHangar::StartShipConstruction(const FString& ShipDesign)
{
	if (!HasAuthority())
		return;

	if (!CurrentShipDesign.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already constructing a ship!"));
		return;
	}

	CurrentShipDesign = ShipDesign;
	ConstructionProgress = 0.0f;
	
	UE_LOG(LogTemp, Log, TEXT("Started ship construction"));
}

AShip* AHangar::FinishShipConstruction()
{
	if (!HasAuthority())
		return nullptr;

	if (ConstructionProgress < 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ship construction not complete!"));
		return nullptr;
	}

	// Spawn ship at hangar location
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 200);
	AShip* NewShip = GetWorld()->SpawnActor<AShip>(AShip::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	
	if (NewShip)
	{
		NewShip->LoadShipDesign(CurrentShipDesign);
		CurrentShipDesign.Empty();
		ConstructionProgress = 0.0f;
	}

	return NewShip;
}

bool AHangar::StoreShip(AShip* Ship)
{
	if (!HasAuthority())
		return false;

	if (!Ship)
		return false;

	if (StoredShipDesigns.Num() >= MaxStoredShips)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hangar is full!"));
		return false;
	}

	// Serialize and store ship design
	FString ShipDesign = Ship->SerializeShipDesign();
	StoredShipDesigns.Add(ShipDesign);
	
	// Destroy the ship actor
	Ship->Destroy();
	
	UE_LOG(LogTemp, Log, TEXT("Ship stored in hangar"));
	return true;
}

AShip* AHangar::DeployShip(int32 ShipIndex)
{
	if (!HasAuthority())
		return nullptr;

	if (ShipIndex < 0 || ShipIndex >= StoredShipDesigns.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ship index!"));
		return nullptr;
	}

	// Spawn ship from stored design
	FString ShipDesign = StoredShipDesigns[ShipIndex];
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 200);
	AShip* NewShip = GetWorld()->SpawnActor<AShip>(AShip::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	
	if (NewShip)
	{
		NewShip->LoadShipDesign(ShipDesign);
		StoredShipDesigns.RemoveAt(ShipIndex);
	}

	return NewShip;
}

float AHangar::GetConstructionProgress() const
{
	return ConstructionProgress;
}
