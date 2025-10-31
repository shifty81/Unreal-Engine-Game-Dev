// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingSystem.generated.h"

/**
 * Base building class for constructible structures
 */
UCLASS()
class VOXELSURVIVAL_API ABuilding : public AActor
{
	GENERATED_BODY()

public:
	ABuilding();

	/** Building name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingName;

	/** Building health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Building")
	float Health = 1000.0f;

	/** Max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float MaxHealth = 1000.0f;

	/** Owner player ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Building")
	int32 OwnerPlayerID = -1;

	/** Influence radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float InfluenceRadius = 500.0f;

	/** Influence amount per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float InfluencePerSecond = 1.0f;

	/** Take damage */
	UFUNCTION(BlueprintCallable, Category = "Building")
	void TakeDamageCustom(float DamageAmount);

	/** Repair building */
	UFUNCTION(BlueprintCallable, Category = "Building")
	void Repair(float RepairAmount);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	UStaticMeshComponent* BuildingMesh;

	/** Apply influence to territory */
	void ApplyInfluence(float DeltaTime);
};
