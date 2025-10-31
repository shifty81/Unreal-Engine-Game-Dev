// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleMod.h"

#define LOCTEXT_NAMESPACE "FExampleModModule"

void FExampleModModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
	UE_LOG(LogTemp, Log, TEXT("ExampleMod module has started"));
	
	// Register custom voxel types, buildings, etc. here
}

void FExampleModModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
	UE_LOG(LogTemp, Log, TEXT("ExampleMod module has shut down"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExampleModModule, ExampleMod)
