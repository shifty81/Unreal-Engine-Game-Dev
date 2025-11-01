// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AResourceManager::AResourceManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	SpawnRadius = 5000.0f;
	SpawnCheckInterval = 10.0f;

	// Enable replication for multiplayer
	bReplicates = true;
}

void AResourceManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Only spawn resources on server
	if (HasAuthority())
	{
		// Initial resource spawn
		SpawnResourcesInArea(GetActorLocation(), SpawnRadius);

		// Set up periodic spawn checks
		GetWorld()->GetTimerManager().SetTimer(
			SpawnCheckTimerHandle,
			this,
			&AResourceManager::PerformSpawnCheck,
			SpawnCheckInterval,
			true
		);
	}
}

void AResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AResourceManager::SpawnResourcesInArea(FVector Center, float Radius)
{
	if (!HasAuthority())
	{
		return;
	}

	for (const FResourceSpawnInfo& SpawnInfo : ResourceSpawnConfigs)
	{
		if (!SpawnInfo.ResourceClass)
		{
			continue;
		}

		// Calculate how many resources to spawn based on area and density
		float Area = PI * Radius * Radius;
		int32 TargetCount = FMath::FloorToInt(Area * SpawnInfo.SpawnDensity);
		TargetCount = FMath::Min(TargetCount, SpawnInfo.MaxCount);

		// Count existing resources of this type
		int32 CurrentCount = 0;
		for (AResourceNode* Resource : SpawnedResources)
		{
			if (Resource && Resource->GetClass() == SpawnInfo.ResourceClass)
			{
				CurrentCount++;
			}
		}

		// Spawn new resources if under target count
		int32 ToSpawn = TargetCount - CurrentCount;
		for (int32 i = 0; i < ToSpawn; i++)
		{
			// Generate random location within radius
			FVector2D RandomCircle = FMath::RandPointInCircle(Radius);
			FVector SpawnLocation = Center + FVector(RandomCircle.X, RandomCircle.Y, 0.0f);

			// Get surface location
			FVector SurfaceLocation;
			if (GetSurfaceLocation(SpawnLocation, SurfaceLocation))
			{
				// Check if within height constraints
				if (SurfaceLocation.Z >= SpawnInfo.MinSpawnHeight && 
					SurfaceLocation.Z <= SpawnInfo.MaxSpawnHeight)
				{
					// Check if location is valid (not too close to other resources)
					if (IsValidSpawnLocation(SurfaceLocation, SpawnInfo.MinSpawnDistance))
					{
						AResourceNode* NewResource = SpawnResourceAtLocation(
							SpawnInfo.ResourceClass, 
							SurfaceLocation
						);
						
						if (NewResource)
						{
							SpawnedResources.Add(NewResource);
						}
					}
				}
			}
		}
	}
}

AResourceNode* AResourceManager::SpawnResourceAtLocation(TSubclassOf<AResourceNode> ResourceClass, FVector Location)
{
	if (!HasAuthority() || !ResourceClass)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AResourceNode* NewResource = GetWorld()->SpawnActor<AResourceNode>(
		ResourceClass,
		Location,
		FRotator::ZeroRotator,
		SpawnParams
	);

	return NewResource;
}

void AResourceManager::ClearAllResources()
{
	for (AResourceNode* Resource : SpawnedResources)
	{
		if (Resource)
		{
			Resource->Destroy();
		}
	}
	SpawnedResources.Empty();
}

int32 AResourceManager::GetResourceCount(EResourceType ResourceType) const
{
	int32 Count = 0;
	for (AResourceNode* Resource : SpawnedResources)
	{
		if (Resource && Resource->ResourceType == ResourceType)
		{
			Count++;
		}
	}
	return Count;
}

AResourceNode* AResourceManager::FindNearestResource(FVector Location, EResourceType ResourceType, float MaxDistance) const
{
	AResourceNode* NearestResource = nullptr;
	float NearestDistanceSq = MaxDistance * MaxDistance;

	for (AResourceNode* Resource : SpawnedResources)
	{
		if (Resource && Resource->ResourceType == ResourceType && !Resource->IsDepleted())
		{
			float DistanceSq = FVector::DistSquared(Location, Resource->GetActorLocation());
			if (DistanceSq < NearestDistanceSq)
			{
				NearestDistanceSq = DistanceSq;
				NearestResource = Resource;
			}
		}
	}

	return NearestResource;
}

void AResourceManager::PerformSpawnCheck()
{
	// Clean up destroyed resources
	SpawnedResources.RemoveAll([](AResourceNode* Resource) {
		return Resource == nullptr || !IsValid(Resource);
	});

	// Find players and spawn resources around them
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), Players);

	for (AActor* Player : Players)
	{
		if (Player)
		{
			SpawnResourcesInArea(Player->GetActorLocation(), SpawnRadius);
		}
	}
}

bool AResourceManager::IsValidSpawnLocation(FVector Location, float MinDistance) const
{
	float MinDistanceSq = MinDistance * MinDistance;

	for (AResourceNode* Resource : SpawnedResources)
	{
		if (Resource)
		{
			float DistanceSq = FVector::DistSquared(Location, Resource->GetActorLocation());
			if (DistanceSq < MinDistanceSq)
			{
				return false;
			}
		}
	}

	return true;
}

bool AResourceManager::GetSurfaceLocation(FVector Location, FVector& OutSurfaceLocation) const
{
	// Trace down to find ground
	FVector Start = Location + FVector(0.0f, 0.0f, 1000.0f);
	FVector End = Location - FVector(0.0f, 0.0f, 2000.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		QueryParams))
	{
		OutSurfaceLocation = HitResult.Location;
		return true;
	}

	// If no hit, use original location
	OutSurfaceLocation = Location;
	return false;
}
