// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceNode.generated.h"

/**
 * Enum defining the types of resources available in the game
 */
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Tree			UMETA(DisplayName = "Tree"),
	Rock			UMETA(DisplayName = "Rock"),
	IronOre			UMETA(DisplayName = "Iron Ore"),
	GoldOre			UMETA(DisplayName = "Gold Ore"),
	CopperOre		UMETA(DisplayName = "Copper Ore"),
	AluminumOre		UMETA(DisplayName = "Aluminum Ore"),
	TitaniumOre		UMETA(DisplayName = "Titanium Ore"),
	CoalOre			UMETA(DisplayName = "Coal Ore")
};

/**
 * Enum defining material types (raw and processed)
 */
UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	// Raw materials
	Wood			UMETA(DisplayName = "Wood"),
	Stone			UMETA(DisplayName = "Stone"),
	IronOre			UMETA(DisplayName = "Iron Ore"),
	CopperOre		UMETA(DisplayName = "Copper Ore"),
	GoldOre			UMETA(DisplayName = "Gold Ore"),
	AluminumOre		UMETA(DisplayName = "Aluminum Ore"),
	TitaniumOre		UMETA(DisplayName = "Titanium Ore"),
	Coal			UMETA(DisplayName = "Coal"),
	
	// Processed materials
	Iron			UMETA(DisplayName = "Iron"),
	Copper			UMETA(DisplayName = "Copper"),
	Gold			UMETA(DisplayName = "Gold"),
	Aluminum		UMETA(DisplayName = "Aluminum"),
	Titanium		UMETA(DisplayName = "Titanium"),
	Steel			UMETA(DisplayName = "Steel"),
	Charcoal		UMETA(DisplayName = "Charcoal")
};

/**
 * Base class for all harvestable resource nodes in the game
 * Resources can be gathered by players and provide materials
 */
UCLASS()
class VOXELSURVIVAL_API AResourceNode : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceNode();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** The type of resource this node represents */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	EResourceType ResourceType;

	/** Maximum amount of resource this node can provide */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 MaxResourceAmount;

	/** Current amount of resource available */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource", Replicated)
	int32 CurrentResourceAmount;

	/** Time in seconds for the resource to respawn after depletion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	float RespawnTime;

	/** Whether this resource respawns after being depleted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	bool bRespawns;

	/** Visual mesh for the resource */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	class UStaticMeshComponent* MeshComponent;

	/**
	 * Harvest resources from this node
	 * @param Amount - Amount to harvest
	 * @return Actual amount harvested
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	int32 HarvestResource(int32 Amount);

	/**
	 * Check if the resource is depleted
	 */
	UFUNCTION(BlueprintPure, Category = "Resource")
	bool IsDepleted() const;

	/** Called when the resource is depleted */
	UFUNCTION(BlueprintNativeEvent, Category = "Resource")
	void OnDepleted();

	/** Called when the resource respawns */
	UFUNCTION(BlueprintNativeEvent, Category = "Resource")
	void OnRespawn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Timer handle for respawn */
	FTimerHandle RespawnTimerHandle;

	/** Handle respawning logic */
	void HandleRespawn();
};
