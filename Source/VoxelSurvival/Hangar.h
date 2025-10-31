// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem.h"
#include "Ship.h"
#include "Hangar.generated.h"

/**
 * Hangar building for ship construction and storage
 */
UCLASS()
class VOXELSURVIVAL_API AHangar : public ABuilding
{
	GENERATED_BODY()

public:
	AHangar();

	/** Ships stored in hangar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Hangar")
	TArray<FString> StoredShipDesigns;

	/** Maximum ships that can be stored */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangar")
	int32 MaxStoredShips = 10;

	/** Currently building ship */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Hangar")
	AShip* CurrentShip;

	/** Start building a ship */
	UFUNCTION(BlueprintCallable, Category = "Hangar")
	void StartShipConstruction(const FString& ShipDesign);

	/** Finish building current ship */
	UFUNCTION(BlueprintCallable, Category = "Hangar")
	AShip* FinishShipConstruction();

	/** Store ship in hangar */
	UFUNCTION(BlueprintCallable, Category = "Hangar")
	bool StoreShip(AShip* Ship);

	/** Deploy ship from hangar */
	UFUNCTION(BlueprintCallable, Category = "Hangar")
	AShip* DeployShip(int32 ShipIndex);

	/** Get construction progress */
	UFUNCTION(BlueprintCallable, Category = "Hangar")
	float GetConstructionProgress() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Construction progress (0-1) */
	UPROPERTY(Replicated)
	float ConstructionProgress = 0.0f;

	/** Construction speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangar")
	float ConstructionSpeed = 0.1f;

	/** Ship design being built */
	FString CurrentShipDesign;
};
