// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem.h"
#include "ExampleBuilding.generated.h"

/**
 * Example custom building for demonstration
 * Shows how to extend the building system
 */
UCLASS()
class EXAMPLEMOD_API AExampleBuilding : public ABuilding
{
	GENERATED_BODY()

public:
	AExampleBuilding();

	/** Example custom property */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
	float CustomValue = 100.0f;

	/** Example custom function */
	UFUNCTION(BlueprintCallable, Category = "Example")
	void DoCustomAction();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/** Example timer */
	float ActionTimer = 0.0f;
};
