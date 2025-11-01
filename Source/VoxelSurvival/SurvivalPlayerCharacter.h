// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "SurvivalPlayerCharacter.generated.h"

UCLASS()
class VOXELSURVIVAL_API ASurvivalPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASurvivalPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Camera components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	/** Crafting component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	class UCraftingComponent* CraftingComponent;

	/** Mesh for equipped item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	class UStaticMeshComponent* EquippedItemMesh;

	/** Currently equipped item */
	UPROPERTY(BlueprintReadOnly, Category = "Equipment", Replicated)
	FInventoryItem CurrentlyEquippedItem;

	/** Movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** Player stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", Replicated)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", Replicated)
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", Replicated)
	float Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", Replicated)
	float Thirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxThirst;

	/** Interaction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange;

	/** Current interaction target */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractionTarget;

	/**
	 * Equip item from inventory slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(int32 InventorySlot);

	/**
	 * Unequip current item
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipItem();

	/**
	 * Use equipped item (harvest, attack, etc)
	 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UseEquippedItem();

	/**
	 * Interact with object in front of player
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/**
	 * Harvest resource node
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HarvestResource(class AResourceNode* ResourceNode);

	/**
	 * Take damage
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void TakeDamageCustom(float DamageAmount);

	/**
	 * Heal
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void Heal(float HealAmount);

	/**
	 * Check if player can sprint
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool CanSprint() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/** Input handlers */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void SelectHotbarSlot1();
	void SelectHotbarSlot2();
	void SelectHotbarSlot3();
	void SelectHotbarSlot4();
	void SelectHotbarSlot5();
	void SelectHotbarSlot6();
	void SelectHotbarSlot7();
	void SelectHotbarSlot8();
	void SelectHotbarSlot9();
	void SelectHotbarSlot0();
	void ToggleInventory();

	/** Update interaction target */
	void UpdateInteractionTarget();

	/** Update stats over time */
	void UpdateStats(float DeltaTime);

	/** Server RPCs */
	UFUNCTION(Server, Reliable)
	void ServerEquipItem(int32 InventorySlot);

	UFUNCTION(Server, Reliable)
	void ServerUseEquippedItem();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	UFUNCTION(Server, Reliable)
	void ServerHarvestResource(AResourceNode* ResourceNode);

private:
	bool bIsSprinting;
	bool bInventoryOpen;

	/** Update equipped item mesh */
	void UpdateEquippedItemMesh();
};
