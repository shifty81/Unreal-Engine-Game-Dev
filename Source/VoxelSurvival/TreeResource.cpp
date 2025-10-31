// Copyright Epic Games, Inc. All Rights Reserved.

#include "TreeResource.h"
#include "Components/StaticMeshComponent.h"

ATreeResource::ATreeResource()
{
	// Set resource type to tree
	ResourceType = EResourceType::Tree;

	// Default tree properties
	TreeHeight = 400.0f;
	TrunkRadius = 30.0f;
	CanopyRadius = 200.0f;
	TreeVariant = 0;

	// Higher resource amount for trees
	MaxResourceAmount = 150;
	CurrentResourceAmount = MaxResourceAmount;
	RespawnTime = 600.0f; // 10 minutes for trees

	// Create foliage mesh component for leaves
	FoliageMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoliageMeshComponent"));
	FoliageMeshComponent->SetupAttachment(RootComponent);
}

void ATreeResource::BeginPlay()
{
	Super::BeginPlay();
	
	// Randomize tree appearance on spawn
	RandomizeTreeProperties();
	GenerateTreeGeometry();
}

void ATreeResource::GenerateTreeGeometry()
{
	// Set trunk scale based on properties
	if (MeshComponent)
	{
		FVector TrunkScale = FVector(
			TrunkRadius / 50.0f,
			TrunkRadius / 50.0f,
			TreeHeight / 400.0f
		);
		MeshComponent->SetRelativeScale3D(TrunkScale);
	}

	// Set foliage scale and position
	if (FoliageMeshComponent)
	{
		FVector CanopyScale = FVector(
			CanopyRadius / 200.0f,
			CanopyRadius / 200.0f,
			CanopyRadius / 200.0f
		);
		FoliageMeshComponent->SetRelativeScale3D(CanopyScale);
		
		// Position foliage at top of trunk
		FVector CanopyPosition = FVector(0.0f, 0.0f, TreeHeight * 0.7f);
		FoliageMeshComponent->SetRelativeLocation(CanopyPosition);
	}
}

void ATreeResource::RandomizeTreeProperties()
{
	// Add some random variation to tree dimensions
	float HeightVariation = FMath::RandRange(-50.0f, 100.0f);
	float RadiusVariation = FMath::RandRange(-5.0f, 10.0f);
	float CanopyVariation = FMath::RandRange(-30.0f, 50.0f);
	
	TreeHeight = FMath::Clamp(TreeHeight + HeightVariation, 100.0f, 1000.0f);
	TrunkRadius = FMath::Clamp(TrunkRadius + RadiusVariation, 10.0f, 100.0f);
	CanopyRadius = FMath::Clamp(CanopyRadius + CanopyVariation, 100.0f, 500.0f);
	
	// Select random tree variant (0-3)
	TreeVariant = FMath::RandRange(0, 3);
}

void ATreeResource::OnDepleted_Implementation()
{
	Super::OnDepleted_Implementation();
	
	// Also hide foliage when depleted
	if (FoliageMeshComponent)
	{
		FoliageMeshComponent->SetVisibility(false);
	}
}

void ATreeResource::OnRespawn_Implementation()
{
	Super::OnRespawn_Implementation();
	
	// Show foliage again
	if (FoliageMeshComponent)
	{
		FoliageMeshComponent->SetVisibility(true);
	}
	
	// Randomize properties for respawned tree
	RandomizeTreeProperties();
	GenerateTreeGeometry();
}
