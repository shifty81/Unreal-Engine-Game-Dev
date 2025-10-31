// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ship.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication
	bReplicates = true;
	SetReplicateMovement(true);

	// Create ship mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Add floating pawn movement
	UFloatingPawnMovement* Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = ShipSpeed;
}

void AShip::BeginPlay()
{
	Super::BeginPlay();
	RebuildShip();
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShip, ShipName);
	DOREPLIFETIME(AShip, Health);
	DOREPLIFETIME(AShip, Components);
}

void AShip::AddComponent(const FShipComponent& Component)
{
	if (!HasAuthority())
		return;

	Components.Add(Component);
	RebuildShip();
}

void AShip::RemoveComponent(int32 ComponentIndex)
{
	if (!HasAuthority())
		return;

	if (ComponentIndex >= 0 && ComponentIndex < Components.Num())
	{
		Components.RemoveAt(ComponentIndex);
		RebuildShip();
	}
}

void AShip::RebuildShip()
{
	// Clear existing component meshes
	for (UStaticMeshComponent* Mesh : ComponentMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	ComponentMeshes.Empty();

	// Create meshes for each component
	for (int32 i = 0; i < Components.Num(); i++)
	{
		const FShipComponent& Component = Components[i];
		
		FString ComponentName = FString::Printf(TEXT("ShipComponent_%d"), i);
		UStaticMeshComponent* ComponentMesh = NewObject<UStaticMeshComponent>(this, *ComponentName);
		
		if (ComponentMesh)
		{
			ComponentMesh->RegisterComponent();
			ComponentMesh->AttachToComponent(ShipMesh, FAttachmentTransformRules::KeepRelativeTransform);
			ComponentMesh->SetRelativeLocation(Component.RelativePosition);
			ComponentMesh->SetRelativeRotation(Component.RelativeRotation);
			ComponentMesh->SetRelativeScale3D(Component.Scale);
			
			ComponentMeshes.Add(ComponentMesh);
		}
	}
}

FString AShip::SerializeShipDesign()
{
	FString DesignData;
	DesignData += FString::Printf(TEXT("Name:%s;"), *ShipName);
	
	for (const FShipComponent& Component : Components)
	{
		DesignData += FString::Printf(TEXT("Component:%s,%f,%f,%f,%f,%f,%f;"),
			*Component.ComponentName,
			Component.RelativePosition.X,
			Component.RelativePosition.Y,
			Component.RelativePosition.Z,
			Component.RelativeRotation.Pitch,
			Component.RelativeRotation.Yaw,
			Component.RelativeRotation.Roll
		);
	}
	
	return DesignData;
}

void AShip::LoadShipDesign(const FString& DesignData)
{
	if (!HasAuthority())
		return;

	Components.Empty();
	
	// Parse design data (simplified implementation)
	TArray<FString> Parts;
	DesignData.ParseIntoArray(Parts, TEXT(";"), true);
	
	for (const FString& Part : Parts)
	{
		if (Part.StartsWith(TEXT("Name:")))
		{
			ShipName = Part.RightChop(5);
		}
		else if (Part.StartsWith(TEXT("Component:")))
		{
			// Parse component data
			FString ComponentData = Part.RightChop(10);
			TArray<FString> Values;
			ComponentData.ParseIntoArray(Values, TEXT(","), true);
			
			if (Values.Num() >= 7)
			{
				FShipComponent Component;
				Component.ComponentName = Values[0];
				Component.RelativePosition = FVector(
					FCString::Atof(*Values[1]),
					FCString::Atof(*Values[2]),
					FCString::Atof(*Values[3])
				);
				Component.RelativeRotation = FRotator(
					FCString::Atof(*Values[4]),
					FCString::Atof(*Values[5]),
					FCString::Atof(*Values[6])
				);
				
				Components.Add(Component);
			}
		}
	}
	
	RebuildShip();
}
