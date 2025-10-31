// Copyright Epic Games, Inc. All Rights Reserved.

#include "BuildingSystem.h"
#include "TerritorySystem.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication
	bReplicates = true;

	// Create mesh component
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = BuildingMesh;
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ApplyInfluence(DeltaTime);
	}
}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuilding, Health);
	DOREPLIFETIME(ABuilding, OwnerPlayerID);
}

void ABuilding::ApplyInfluence(float DeltaTime)
{
	// Find territory system
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATerritorySystem::StaticClass(), FoundActors);
	
	if (FoundActors.Num() > 0)
	{
		ATerritorySystem* TerritorySystem = Cast<ATerritorySystem>(FoundActors[0]);
		if (TerritorySystem && OwnerPlayerID >= 0)
		{
			// Add influence in radius around building
			FVector BuildingLocation = GetActorLocation();
			float InfluenceToAdd = InfluencePerSecond * DeltaTime;
			
			TerritorySystem->AddInfluence(BuildingLocation, OwnerPlayerID, InfluenceToAdd);
		}
	}
}

void ABuilding::TakeDamageCustom(float DamageAmount)
{
	if (!HasAuthority())
		return;

	Health = FMath::Max(0.0f, Health - DamageAmount);
	
	if (Health <= 0.0f)
	{
		// Destroy building
		Destroy();
	}
}

void ABuilding::Repair(float RepairAmount)
{
	if (!HasAuthority())
		return;

	Health = FMath::Min(MaxHealth, Health + RepairAmount);
}
