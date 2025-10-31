// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

/**
 * Ship component data structure
 */
USTRUCT(BlueprintType)
struct FShipComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ComponentType = 0;
};

/**
 * Modular ship that can be built with custom components
 */
UCLASS()
class VOXELSURVIVAL_API AShip : public APawn
{
	GENERATED_BODY()

public:
	AShip();

	/** Ship name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Ship")
	FString ShipName;

	/** Ship health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Ship")
	float Health = 1000.0f;

	/** Max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship")
	float MaxHealth = 1000.0f;

	/** Ship speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship")
	float ShipSpeed = 1000.0f;

	/** Ship components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Ship")
	TArray<FShipComponent> Components;

	/** Add component to ship */
	UFUNCTION(BlueprintCallable, Category = "Ship")
	void AddComponent(const FShipComponent& Component);

	/** Remove component from ship */
	UFUNCTION(BlueprintCallable, Category = "Ship")
	void RemoveComponent(int32 ComponentIndex);

	/** Rebuild ship mesh from components */
	UFUNCTION(BlueprintCallable, Category = "Ship")
	void RebuildShip();

	/** Serialize ship design for saving */
	UFUNCTION(BlueprintCallable, Category = "Ship")
	FString SerializeShipDesign();

	/** Load ship design from data */
	UFUNCTION(BlueprintCallable, Category = "Ship")
	void LoadShipDesign(const FString& DesignData);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Ship root mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship")
	UStaticMeshComponent* ShipMesh;

	/** Dynamic component meshes */
	UPROPERTY()
	TArray<UStaticMeshComponent*> ComponentMeshes;
};
