// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VoxelSurvivalGameMode.generated.h"

/**
 * Game mode for voxel survival game with RTS elements
 * Manages multiplayer sessions and game state
 */
UCLASS()
class VOXELSURVIVAL_API AVoxelSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVoxelSurvivalGameMode();

	/** Initialize game for server */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Post login - called after player joins */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Player logout */
	virtual void Logout(AController* Exiting) override;

protected:
	/** Territory system reference */
	UPROPERTY()
	class ATerritorySystem* TerritorySystem;

	/** Voxel world reference */
	UPROPERTY()
	class AVoxelWorld* VoxelWorld;

	/** Next player ID */
	int32 NextPlayerID = 0;
};
