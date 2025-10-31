// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerritorySystem.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"

ATerritorySystem::ATerritorySystem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Enable replication for multiplayer
	bReplicates = true;
}

void ATerritorySystem::BeginPlay()
{
	Super::BeginPlay();
}

void ATerritorySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update territories periodically (every 5 seconds)
	static float UpdateTimer = 0.0f;
	UpdateTimer += DeltaTime;
	
	if (UpdateTimer >= 5.0f && HasAuthority())
	{
		UpdateTerritories();
		UpdateTimer = 0.0f;
	}
}

void ATerritorySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATerritorySystem, TerritoryOwnership);
}

FIntVector ATerritorySystem::WorldToGridCoordinate(FVector Position) const
{
	return FIntVector(
		FMath::FloorToInt(Position.X / TerritoryGridSize),
		FMath::FloorToInt(Position.Y / TerritoryGridSize),
		0
	);
}

int32 ATerritorySystem::GetTerritoryOwner(FVector Position) const
{
	FIntVector GridCoord = WorldToGridCoordinate(Position);
	
	if (TerritoryOwnership.Contains(GridCoord))
	{
		return TerritoryOwnership[GridCoord];
	}
	
	return -1; // No owner
}

void ATerritorySystem::SetTerritoryOwner(FVector Position, int32 OwnerID)
{
	if (!HasAuthority())
		return;

	FIntVector GridCoord = WorldToGridCoordinate(Position);
	TerritoryOwnership.Add(GridCoord, OwnerID);
}

float ATerritorySystem::GetInfluence(FVector Position, int32 PlayerID) const
{
	FIntVector GridCoord = WorldToGridCoordinate(Position);
	
	if (InfluenceMap.Contains(GridCoord))
	{
		const TMap<int32, float>& PlayerInfluence = InfluenceMap[GridCoord];
		if (PlayerInfluence.Contains(PlayerID))
		{
			return PlayerInfluence[PlayerID];
		}
	}
	
	return 0.0f;
}

void ATerritorySystem::AddInfluence(FVector Position, int32 PlayerID, float InfluenceAmount)
{
	if (!HasAuthority())
		return;

	FIntVector GridCoord = WorldToGridCoordinate(Position);
	
	if (!InfluenceMap.Contains(GridCoord))
	{
		InfluenceMap.Add(GridCoord, TMap<int32, float>());
	}
	
	TMap<int32, float>& PlayerInfluence = InfluenceMap[GridCoord];
	float CurrentInfluence = PlayerInfluence.Contains(PlayerID) ? PlayerInfluence[PlayerID] : 0.0f;
	PlayerInfluence.Add(PlayerID, CurrentInfluence + InfluenceAmount);
}

void ATerritorySystem::UpdateTerritories()
{
	if (!HasAuthority())
		return;

	// Update territory ownership based on influence
	for (auto& Pair : InfluenceMap)
	{
		FIntVector GridCoord = Pair.Key;
		TMap<int32, float>& PlayerInfluence = Pair.Value;
		
		// Find player with highest influence
		int32 MaxInfluencePlayer = -1;
		float MaxInfluence = 0.0f;
		
		for (auto& InfluencePair : PlayerInfluence)
		{
			if (InfluencePair.Value > MaxInfluence)
			{
				MaxInfluence = InfluencePair.Value;
				MaxInfluencePlayer = InfluencePair.Key;
			}
		}
		
		// Set territory owner if influence threshold is met
		if (MaxInfluence > 10.0f)
		{
			TerritoryOwnership.Add(GridCoord, MaxInfluencePlayer);
		}
	}
}

bool ATerritorySystem::CanBuildAtPosition(FVector Position, int32 PlayerID) const
{
	int32 Owner = GetTerritoryOwner(Position);
	
	// Can build if no owner or if player owns territory
	return Owner == -1 || Owner == PlayerID;
}
