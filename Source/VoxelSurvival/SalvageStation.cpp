// Copyright Epic Games, Inc. All Rights Reserved.

#include "SalvageStation.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

ASalvageStation::ASalvageStation()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create station mesh component
	StationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMeshComponent"));
	RootComponent = StationMeshComponent;

	// Default values
	bIsProcessing = false;
	ProcessingTimeRemaining = 0.0f;

	// Enable replication
	bReplicates = true;
}

void ASalvageStation::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize default salvage recipes
	InitializeDefaultSalvageRecipes();
}

void ASalvageStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Process salvaging on server
	if (HasAuthority() && bIsProcessing)
	{
		ProcessSalvaging(DeltaTime);
	}
}

void ASalvageStation::AddInputMaterial(EMaterialType MaterialType, int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	if (InputInventory.Contains(MaterialType))
	{
		InputInventory[MaterialType] += Amount;
	}
	else
	{
		InputInventory.Add(MaterialType, Amount);
	}
}

bool ASalvageStation::StartSalvaging(EMaterialType InputMaterial)
{
	if (!HasAuthority() || bIsProcessing)
	{
		return false;
	}

	// Check if we have the input material
	if (!InputInventory.Contains(InputMaterial) || InputInventory[InputMaterial] <= 0)
	{
		return false;
	}

	// Get salvage recipe
	FSalvageRecipe Recipe;
	if (!GetSalvageRecipe(InputMaterial, Recipe))
	{
		return false;
	}

	// Consume input material
	InputInventory[InputMaterial]--;
	if (InputInventory[InputMaterial] <= 0)
	{
		InputInventory.Remove(InputMaterial);
	}

	// Start processing
	CurrentRecipe = Recipe;
	bIsProcessing = true;
	ProcessingTimeRemaining = Recipe.SalvageTime;

	return true;
}

TMap<EMaterialType, int32> ASalvageStation::CollectOutput()
{
	TMap<EMaterialType, int32> CollectedOutput = OutputInventory;
	OutputInventory.Empty();
	return CollectedOutput;
}

bool ASalvageStation::GetSalvageRecipe(EMaterialType InputMaterial, FSalvageRecipe& OutRecipe) const
{
	for (const FSalvageRecipe& Recipe : SalvageRecipes)
	{
		if (Recipe.InputMaterial == InputMaterial)
		{
			OutRecipe = Recipe;
			return true;
		}
	}
	return false;
}

void ASalvageStation::InitializeDefaultSalvageRecipes()
{
	SalvageRecipes.Empty();

	// Wood to Charcoal (with chance)
	FSalvageRecipe WoodToCharcoal;
	WoodToCharcoal.InputMaterial = EMaterialType::Wood;
	WoodToCharcoal.SalvageTime = 5.0f;
	FSalvageOutput CharcoalOutput;
	CharcoalOutput.MaterialType = EMaterialType::Charcoal;
	CharcoalOutput.Amount = 1;
	CharcoalOutput.DropChance = 0.8f; // 80% chance
	WoodToCharcoal.PossibleOutputs.Add(CharcoalOutput);
	SalvageRecipes.Add(WoodToCharcoal);

	// Steel back to Iron (partial recovery)
	FSalvageRecipe SteelToIron;
	SteelToIron.InputMaterial = EMaterialType::Steel;
	SteelToIron.SalvageTime = 8.0f;
	FSalvageOutput IronOutput;
	IronOutput.MaterialType = EMaterialType::Iron;
	IronOutput.Amount = 1;
	IronOutput.DropChance = 1.0f;
	SteelToIron.PossibleOutputs.Add(IronOutput);
	SalvageRecipes.Add(SteelToIron);

	// Complex items can be added here with multiple outputs
	// For example, a steel tool might give back steel and wood handle
}

void ASalvageStation::ProcessSalvaging(float DeltaTime)
{
	ProcessingTimeRemaining -= DeltaTime;

	if (ProcessingTimeRemaining <= 0.0f)
	{
		CompleteSalvaging();
	}
}

void ASalvageStation::CompleteSalvaging()
{
	// Generate outputs based on drop chances
	for (const FSalvageOutput& Output : CurrentRecipe.PossibleOutputs)
	{
		float Roll = FMath::FRand();
		if (Roll <= Output.DropChance)
		{
			// Add to output inventory
			if (OutputInventory.Contains(Output.MaterialType))
			{
				OutputInventory[Output.MaterialType] += Output.Amount;
			}
			else
			{
				OutputInventory.Add(Output.MaterialType, Output.Amount);
			}
		}
	}

	// Reset processing state
	bIsProcessing = false;
	ProcessingTimeRemaining = 0.0f;

	// Auto-start next salvage if there's more of the same input
	if (InputInventory.Contains(CurrentRecipe.InputMaterial) && 
		InputInventory[CurrentRecipe.InputMaterial] > 0)
	{
		StartSalvaging(CurrentRecipe.InputMaterial);
	}
}

void ASalvageStation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASalvageStation, InputInventory);
	DOREPLIFETIME(ASalvageStation, OutputInventory);
	DOREPLIFETIME(ASalvageStation, bIsProcessing);
	DOREPLIFETIME(ASalvageStation, ProcessingTimeRemaining);
}
