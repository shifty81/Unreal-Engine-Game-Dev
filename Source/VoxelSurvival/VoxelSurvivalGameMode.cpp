// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelSurvivalGameMode.h"
#include "WireframeCharacter.h"
#include "TerritorySystem.h"
#include "VoxelWorld.h"
#include "Kismet/GameplayStatics.h"

AVoxelSurvivalGameMode::AVoxelSurvivalGameMode()
{
	// Set default pawn class
	DefaultPawnClass = AWireframeCharacter::StaticClass();
}

void AVoxelSurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Spawn territory system
	FActorSpawnParameters SpawnParams;
	TerritorySystem = GetWorld()->SpawnActor<ATerritorySystem>(ATerritorySystem::StaticClass(), SpawnParams);

	// Spawn voxel world
	VoxelWorld = GetWorld()->SpawnActor<AVoxelWorld>(AVoxelWorld::StaticClass(), SpawnParams);

	UE_LOG(LogTemp, Log, TEXT("VoxelSurvival Game Mode initialized"));
}

void AVoxelSurvivalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Assign player ID
	int32 PlayerID = NextPlayerID++;
	
	UE_LOG(LogTemp, Log, TEXT("Player %d joined the game"), PlayerID);
}

void AVoxelSurvivalGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	UE_LOG(LogTemp, Log, TEXT("Player left the game"));
}
