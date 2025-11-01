// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceNode.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

AResourceNode::AResourceNode()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Default values
	MaxResourceAmount = 100;
	CurrentResourceAmount = MaxResourceAmount;
	RespawnTime = 300.0f; // 5 minutes
	bRespawns = true;
	ResourceType = EResourceType::Tree;

	// Enable replication
	bReplicates = true;
	SetReplicateMovement(false); // Resource nodes don't move
}

void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize resource amount
	CurrentResourceAmount = MaxResourceAmount;
}

void AResourceNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AResourceNode::HarvestResource(int32 Amount)
{
	if (CurrentResourceAmount <= 0)
	{
		return 0;
	}

	// Calculate actual amount to harvest
	int32 HarvestedAmount = FMath::Min(Amount, CurrentResourceAmount);
	CurrentResourceAmount -= HarvestedAmount;

	// Check if depleted
	if (CurrentResourceAmount <= 0)
	{
		OnDepleted();
		
		// Start respawn timer if applicable
		if (bRespawns && HasAuthority())
		{
			GetWorld()->GetTimerManager().SetTimer(
				RespawnTimerHandle,
				this,
				&AResourceNode::HandleRespawn,
				RespawnTime,
				false
			);
		}
	}

	return HarvestedAmount;
}

bool AResourceNode::IsDepleted() const
{
	return CurrentResourceAmount <= 0;
}

void AResourceNode::OnDepleted_Implementation()
{
	// Hide the mesh when depleted
	if (MeshComponent)
	{
		MeshComponent->SetVisibility(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AResourceNode::OnRespawn_Implementation()
{
	// Reset resource amount
	CurrentResourceAmount = MaxResourceAmount;
	
	// Show the mesh again
	if (MeshComponent)
	{
		MeshComponent->SetVisibility(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AResourceNode::HandleRespawn()
{
	OnRespawn();
}

void AResourceNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AResourceNode, CurrentResourceAmount);
}
