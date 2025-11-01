// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "ResourceNode.h"
#include "CraftingComponent.h"

ASurvivalPlayerCharacter::ASurvivalPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Create crafting component
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComponent"));

	// Create equipped item mesh
	EquippedItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedItemMesh"));
	EquippedItemMesh->SetupAttachment(GetMesh(), FName("hand_rSocket")); // Attach to right hand
	EquippedItemMesh->SetVisibility(false);

	// Default values
	WalkSpeed = 600.0f;
	SprintSpeed = 900.0f;
	MaxHealth = 100.0f;
	Health = MaxHealth;
	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	MaxHunger = 100.0f;
	Hunger = MaxHunger;
	MaxThirst = 100.0f;
	Thirst = MaxThirst;
	InteractionRange = 300.0f;
	
	bIsSprinting = false;
	bInventoryOpen = false;
	CurrentInteractionTarget = nullptr;

	// Enable replication
	bReplicates = true;
	SetReplicateMovement(true);
}

void ASurvivalPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASurvivalPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStats(DeltaTime);
	UpdateInteractionTarget();
}

void ASurvivalPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASurvivalPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASurvivalPlayerCharacter::LookUp);

	// Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASurvivalPlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASurvivalPlayerCharacter::StopJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASurvivalPlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASurvivalPlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASurvivalPlayerCharacter::UseEquippedItem);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalPlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ASurvivalPlayerCharacter::ToggleInventory);

	// Hotbar
	PlayerInputComponent->BindAction("Hotbar1", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot1);
	PlayerInputComponent->BindAction("Hotbar2", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot2);
	PlayerInputComponent->BindAction("Hotbar3", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot3);
	PlayerInputComponent->BindAction("Hotbar4", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot4);
	PlayerInputComponent->BindAction("Hotbar5", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot5);
	PlayerInputComponent->BindAction("Hotbar6", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot6);
	PlayerInputComponent->BindAction("Hotbar7", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot7);
	PlayerInputComponent->BindAction("Hotbar8", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot8);
	PlayerInputComponent->BindAction("Hotbar9", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot9);
	PlayerInputComponent->BindAction("Hotbar0", IE_Pressed, this, &ASurvivalPlayerCharacter::SelectHotbarSlot0);
}

void ASurvivalPlayerCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASurvivalPlayerCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASurvivalPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASurvivalPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASurvivalPlayerCharacter::StartJump()
{
	Jump();
}

void ASurvivalPlayerCharacter::StopJump()
{
	StopJumping();
}

void ASurvivalPlayerCharacter::StartSprint()
{
	if (CanSprint())
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASurvivalPlayerCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

bool ASurvivalPlayerCharacter::CanSprint() const
{
	return Stamina > 10.0f && GetVelocity().SizeSquared() > 0.0f;
}

void ASurvivalPlayerCharacter::SelectHotbarSlot1() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(0); EquipItem(0); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot2() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(1); EquipItem(1); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot3() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(2); EquipItem(2); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot4() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(3); EquipItem(3); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot5() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(4); EquipItem(4); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot6() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(5); EquipItem(5); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot7() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(6); EquipItem(6); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot8() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(7); EquipItem(7); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot9() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(8); EquipItem(8); } }
void ASurvivalPlayerCharacter::SelectHotbarSlot0() { if (InventoryComponent) { InventoryComponent->SelectHotbarSlot(9); EquipItem(9); } }

void ASurvivalPlayerCharacter::ToggleInventory()
{
	bInventoryOpen = !bInventoryOpen;
	// UI will handle showing/hiding inventory widget
}

void ASurvivalPlayerCharacter::EquipItem(int32 InventorySlot)
{
	if (HasAuthority())
	{
		if (InventoryComponent)
		{
			FInventoryItem Item = InventoryComponent->GetItemAtSlot(InventorySlot);
			if (Item.IsValid() && Item.bCanEquip)
			{
				CurrentlyEquippedItem = Item;
				UpdateEquippedItemMesh();
			}
			else
			{
				UnequipItem();
			}
		}
	}
	else
	{
		ServerEquipItem(InventorySlot);
	}
}

void ASurvivalPlayerCharacter::ServerEquipItem_Implementation(int32 InventorySlot)
{
	EquipItem(InventorySlot);
}

void ASurvivalPlayerCharacter::UnequipItem()
{
	CurrentlyEquippedItem = FInventoryItem();
	if (EquippedItemMesh)
	{
		EquippedItemMesh->SetVisibility(false);
	}
}

void ASurvivalPlayerCharacter::UseEquippedItem()
{
	if (HasAuthority())
	{
		ServerUseEquippedItem();
	}
	else
	{
		ServerUseEquippedItem();
	}
}

void ASurvivalPlayerCharacter::ServerUseEquippedItem_Implementation()
{
	if (!CurrentlyEquippedItem.IsValid())
	{
		return;
	}

	// Check for resource nodes in range
	if (CurrentInteractionTarget && CurrentInteractionTarget->IsA<AResourceNode>())
	{
		AResourceNode* ResourceNode = Cast<AResourceNode>(CurrentInteractionTarget);
		if (ResourceNode)
		{
			HarvestResource(ResourceNode);
		}
	}
}

void ASurvivalPlayerCharacter::Interact()
{
	if (HasAuthority())
	{
		ServerInteract();
	}
	else
	{
		ServerInteract();
	}
}

void ASurvivalPlayerCharacter::ServerInteract_Implementation()
{
	if (CurrentInteractionTarget)
	{
		// Handle different interaction types
		if (AResourceNode* ResourceNode = Cast<AResourceNode>(CurrentInteractionTarget))
		{
			HarvestResource(ResourceNode);
		}
		// Add more interaction types here (doors, chests, NPCs, etc.)
	}
}

void ASurvivalPlayerCharacter::HarvestResource(AResourceNode* ResourceNode)
{
	if (!ResourceNode || ResourceNode->IsDepleted())
	{
		return;
	}

	float HarvestPower = 1.0f;
	if (CurrentlyEquippedItem.IsValid())
	{
		HarvestPower = CurrentlyEquippedItem.HarvestPower;
	}

	int32 HarvestAmount = FMath::FloorToInt(HarvestPower * 5.0f); // 5 base harvest
	int32 Harvested = ResourceNode->HarvestResource(HarvestAmount);

	if (Harvested > 0 && InventoryComponent)
	{
		// Determine material type based on resource type
		EMaterialType MaterialType = EMaterialType::Wood;
		switch (ResourceNode->ResourceType)
		{
		case EResourceType::Tree:
			MaterialType = EMaterialType::Wood;
			break;
		case EResourceType::Rock:
			MaterialType = EMaterialType::Stone;
			break;
		case EResourceType::IronOre:
			MaterialType = EMaterialType::IronOre;
			break;
		case EResourceType::CopperOre:
			MaterialType = EMaterialType::CopperOre;
			break;
		case EResourceType::GoldOre:
			MaterialType = EMaterialType::GoldOre;
			break;
		case EResourceType::AluminumOre:
			MaterialType = EMaterialType::AluminumOre;
			break;
		case EResourceType::TitaniumOre:
			MaterialType = EMaterialType::TitaniumOre;
			break;
		case EResourceType::CoalOre:
			MaterialType = EMaterialType::Coal;
			break;
		}

		InventoryComponent->AddMaterial(MaterialType, Harvested);

		// Damage tool if equipped
		if (CurrentlyEquippedItem.IsValid() && CurrentlyEquippedItem.MaxDurability > 0)
		{
			CurrentlyEquippedItem.Durability -= 1.0f;
			if (CurrentlyEquippedItem.Durability <= 0.0f)
			{
				// Tool broken
				UnequipItem();
				InventoryComponent->RemoveItem(InventoryComponent->SelectedHotbarSlot, 1);
			}
		}
	}
}

void ASurvivalPlayerCharacter::ServerHarvestResource_Implementation(AResourceNode* ResourceNode)
{
	HarvestResource(ResourceNode);
}

void ASurvivalPlayerCharacter::TakeDamageCustom(float DamageAmount)
{
	Health = FMath::Max(0.0f, Health - DamageAmount);
	
	if (Health <= 0.0f)
	{
		// Handle death
	}
}

void ASurvivalPlayerCharacter::Heal(float HealAmount)
{
	Health = FMath::Min(MaxHealth, Health + HealAmount);
}

void ASurvivalPlayerCharacter::UpdateInteractionTarget()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetForwardVector() * InteractionRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && (HitActor->IsA<AResourceNode>()))
		{
			CurrentInteractionTarget = HitActor;
			return;
		}
	}

	CurrentInteractionTarget = nullptr;
}

void ASurvivalPlayerCharacter::UpdateStats(float DeltaTime)
{
	if (!HasAuthority())
	{
		return;
	}

	// Stamina regeneration
	if (!bIsSprinting)
	{
		Stamina = FMath::Min(MaxStamina, Stamina + (10.0f * DeltaTime));
	}
	else
	{
		Stamina = FMath::Max(0.0f, Stamina - (15.0f * DeltaTime));
		if (Stamina <= 0.0f)
		{
			StopSprint();
		}
	}

	// Hunger and thirst decay
	Hunger = FMath::Max(0.0f, Hunger - (1.0f * DeltaTime));
	Thirst = FMath::Max(0.0f, Thirst - (1.5f * DeltaTime));

	// Take damage from hunger/thirst
	if (Hunger <= 0.0f || Thirst <= 0.0f)
	{
		TakeDamageCustom(5.0f * DeltaTime);
	}
}

void ASurvivalPlayerCharacter::UpdateEquippedItemMesh()
{
	if (!EquippedItemMesh)
	{
		return;
	}

	if (CurrentlyEquippedItem.IsValid() && CurrentlyEquippedItem.ItemMesh.IsValid())
	{
		// Load mesh asynchronously if needed
		UStaticMesh* Mesh = CurrentlyEquippedItem.ItemMesh.LoadSynchronous();
		if (Mesh)
		{
			EquippedItemMesh->SetStaticMesh(Mesh);
			EquippedItemMesh->SetVisibility(true);
		}
	}
	else
	{
		EquippedItemMesh->SetVisibility(false);
	}
}

void ASurvivalPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASurvivalPlayerCharacter, CurrentlyEquippedItem);
	DOREPLIFETIME(ASurvivalPlayerCharacter, Health);
	DOREPLIFETIME(ASurvivalPlayerCharacter, Stamina);
	DOREPLIFETIME(ASurvivalPlayerCharacter, Hunger);
	DOREPLIFETIME(ASurvivalPlayerCharacter, Thirst);
}
