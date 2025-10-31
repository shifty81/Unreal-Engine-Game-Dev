// Copyright Epic Games, Inc. All Rights Reserved.

#include "Forge.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

AForge::AForge()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create forge mesh component
	ForgeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForgeMeshComponent"));
	RootComponent = ForgeMeshComponent;

	// Default values
	MaxCraftingSlots = 3;

	// Enable replication
	bReplicates = true;
}

void AForge::BeginPlay()
{
	Super::BeginPlay();
}

void AForge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Process crafting on server
	if (HasAuthority())
	{
		ProcessCraftingJobs(DeltaTime);
	}
}

void AForge::AddMaterial(EMaterialType MaterialType, int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	if (MaterialInventory.Contains(MaterialType))
	{
		MaterialInventory[MaterialType] += Amount;
	}
	else
	{
		MaterialInventory.Add(MaterialType, Amount);
	}
}

bool AForge::RemoveMaterial(EMaterialType MaterialType, int32 Amount)
{
	if (!MaterialInventory.Contains(MaterialType))
	{
		return false;
	}

	int32 CurrentAmount = MaterialInventory[MaterialType];
	if (CurrentAmount < Amount)
	{
		return false;
	}

	MaterialInventory[MaterialType] = CurrentAmount - Amount;
	
	// Remove from map if depleted
	if (MaterialInventory[MaterialType] <= 0)
	{
		MaterialInventory.Remove(MaterialType);
	}

	return true;
}

bool AForge::HasMaterials(const FCraftingRecipe& Recipe) const
{
	for (const FRecipeIngredient& Ingredient : Recipe.Ingredients)
	{
		int32 Available = GetMaterialCount(Ingredient.MaterialType);
		if (Available < Ingredient.Amount)
		{
			return false;
		}
	}
	return true;
}

bool AForge::StartCrafting(const FCraftingRecipe& Recipe)
{
	if (!HasAuthority())
	{
		return false;
	}

	// Check if we have enough crafting slots
	if (CraftingQueue.Num() >= MaxCraftingSlots)
	{
		return false;
	}

	// Check if we have required materials
	if (!HasMaterials(Recipe))
	{
		return false;
	}

	// Consume materials
	for (const FRecipeIngredient& Ingredient : Recipe.Ingredients)
	{
		RemoveMaterial(Ingredient.MaterialType, Ingredient.Amount);
	}

	// Create crafting job
	FCraftingJob NewJob;
	NewJob.Recipe = Recipe;
	NewJob.TimeRemaining = Recipe.CraftingTime;
	NewJob.bComplete = false;

	CraftingQueue.Add(NewJob);

	return true;
}

void AForge::CancelCrafting(int32 JobIndex)
{
	if (!HasAuthority() || !CraftingQueue.IsValidIndex(JobIndex))
	{
		return;
	}

	// Return materials to inventory
	FCraftingJob& Job = CraftingQueue[JobIndex];
	for (const FRecipeIngredient& Ingredient : Job.Recipe.Ingredients)
	{
		AddMaterial(Ingredient.MaterialType, Ingredient.Amount);
	}

	CraftingQueue.RemoveAt(JobIndex);
}

TMap<EMaterialType, int32> AForge::CollectCompletedItems()
{
	TMap<EMaterialType, int32> CollectedItems = CompletedItems;
	CompletedItems.Empty();
	return CollectedItems;
}

int32 AForge::GetMaterialCount(EMaterialType MaterialType) const
{
	if (MaterialInventory.Contains(MaterialType))
	{
		return MaterialInventory[MaterialType];
	}
	return 0;
}

void AForge::ProcessCraftingJobs(float DeltaTime)
{
	for (int32 i = CraftingQueue.Num() - 1; i >= 0; i--)
	{
		FCraftingJob& Job = CraftingQueue[i];
		
		if (!Job.bComplete)
		{
			Job.TimeRemaining -= DeltaTime;
			
			if (Job.TimeRemaining <= 0.0f)
			{
				CompleteCraftingJob(i);
			}
		}
	}
}

void AForge::CompleteCraftingJob(int32 JobIndex)
{
	if (!CraftingQueue.IsValidIndex(JobIndex))
	{
		return;
	}

	FCraftingJob& Job = CraftingQueue[JobIndex];
	Job.bComplete = true;

	// Add output to completed items
	EMaterialType OutputType = Job.Recipe.OutputMaterial;
	int32 OutputAmount = Job.Recipe.OutputAmount;

	if (CompletedItems.Contains(OutputType))
	{
		CompletedItems[OutputType] += OutputAmount;
	}
	else
	{
		CompletedItems.Add(OutputType, OutputAmount);
	}

	// Remove from queue
	CraftingQueue.RemoveAt(JobIndex);
}

void AForge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AForge, CraftingQueue);
	DOREPLIFETIME(AForge, MaterialInventory);
}
