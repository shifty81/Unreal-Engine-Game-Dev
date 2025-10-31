// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WireframeCharacter.generated.h"

/**
 * Basic wireframe character for the game
 * Includes survival stats and inventory
 */
UCLASS()
class VOXELSURVIVAL_API AWireframeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWireframeCharacter();

	/** Health stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Survival")
	float Health = 100.0f;

	/** Max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival")
	float MaxHealth = 100.0f;

	/** Hunger stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Survival")
	float Hunger = 100.0f;

	/** Thirst stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Survival")
	float Thirst = 100.0f;

	/** Stamina stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Survival")
	float Stamina = 100.0f;

	/** Inventory size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySize = 20;

	/** Take damage */
	UFUNCTION(BlueprintCallable, Category = "Survival")
	void TakeDamageCustom(float DamageAmount);

	/** Heal */
	UFUNCTION(BlueprintCallable, Category = "Survival")
	void Heal(float HealAmount);

	/** Consume food */
	UFUNCTION(BlueprintCallable, Category = "Survival")
	void EatFood(float FoodValue);

	/** Consume water */
	UFUNCTION(BlueprintCallable, Category = "Survival")
	void DrinkWater(float WaterValue);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Update survival stats */
	void UpdateSurvivalStats(float DeltaTime);

	/** Wireframe mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* WireframeMesh;
};
