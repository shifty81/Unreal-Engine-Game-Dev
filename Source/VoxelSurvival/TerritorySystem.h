// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerritorySystem.generated.h"

/**
 * Manages territory control and influence for RTS elements
 */
UCLASS()
class VOXELSURVIVAL_API ATerritorySystem : public AActor
{
	GENERATED_BODY()

public:
	ATerritorySystem();

	/** Territory grid size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory")
	float TerritoryGridSize = 1000.0f;

	/** Get territory owner at position */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	int32 GetTerritoryOwner(FVector Position) const;

	/** Set territory owner */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	void SetTerritoryOwner(FVector Position, int32 OwnerID);

	/** Get influence at position */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	float GetInfluence(FVector Position, int32 PlayerID) const;

	/** Add influence at position */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	void AddInfluence(FVector Position, int32 PlayerID, float InfluenceAmount);

	/** Update territory based on influence */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	void UpdateTerritories();

	/** Check if player can build at position */
	UFUNCTION(BlueprintCallable, Category = "Territory")
	bool CanBuildAtPosition(FVector Position, int32 PlayerID) const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Territory ownership map */
	UPROPERTY(Replicated)
	TMap<FIntVector, int32> TerritoryOwnership;

	/** Influence map for each player */
	TMap<FIntVector, TMap<int32, float>> InfluenceMap;

	/** Convert world position to grid coordinate */
	FIntVector WorldToGridCoordinate(FVector Position) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
