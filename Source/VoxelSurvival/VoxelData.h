// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Voxel block type enumeration */
UENUM(BlueprintType)
enum class EVoxelType : uint8
{
	Air = 0 UMETA(DisplayName = "Air"),
	Stone UMETA(DisplayName = "Stone"),
	Dirt UMETA(DisplayName = "Dirt"),
	Grass UMETA(DisplayName = "Grass"),
	Wood UMETA(DisplayName = "Wood"),
	Iron UMETA(DisplayName = "Iron"),
	Gold UMETA(DisplayName = "Gold"),
	Water UMETA(DisplayName = "Water"),
	WaterSource UMETA(DisplayName = "Water Source"),
	Custom UMETA(DisplayName = "Custom")
};

/** Represents a single voxel in the world */
struct FVoxelData
{
	EVoxelType Type;
	uint8 Health;
	uint8 CustomData;
	uint8 WaterLevel; // 0-8, where 8 is full (for water physics)

	FVoxelData()
		: Type(EVoxelType::Air)
		, Health(100)
		, CustomData(0)
		, WaterLevel(0)
	{}

	FVoxelData(EVoxelType InType)
		: Type(InType)
		, Health(100)
		, CustomData(0)
		, WaterLevel(InType == EVoxelType::WaterSource ? 8 : (InType == EVoxelType::Water ? 7 : 0))
	{}

	bool IsSolid() const
	{
		return Type != EVoxelType::Air && Type != EVoxelType::Water && Type != EVoxelType::WaterSource;
	}

	bool IsTransparent() const
	{
		return Type == EVoxelType::Air || Type == EVoxelType::Water || Type == EVoxelType::WaterSource;
	}

	bool IsWater() const
	{
		return Type == EVoxelType::Water || Type == EVoxelType::WaterSource;
	}
};
