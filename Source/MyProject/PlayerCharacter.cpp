// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "CustomCharacterController.h"
#include "Pickup.h"
#include "Inventory.h"


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("ItemDragModifier", IE_Pressed, this, &APlayerCharacter::ModifyItemDrag);
	PlayerInputComponent->BindAction("ItemDragModifier", IE_Released, this, &APlayerCharacter::StopModifyItemDrag);
}


void APlayerCharacter::Interact()
{
	if (FoundPickup)
	{
		ACustomCharacterController* PlayerController = Cast<ACustomCharacterController>(Controller);
		if (PlayerController->InventoryReference)
		{
			int32 PotentialRest = 0;
			bool bIsItemAdded = PlayerController->InventoryReference->AddItem(FoundPickup->ItemToAdd, FoundPickup->DatabaseKey, FoundPickup->Amount, PotentialRest);

			if (bIsItemAdded && PotentialRest > 0)
			{
				FoundPickup->Amount = PotentialRest;
			}
			else
			{
				FoundPickup->Destroy();
			}
		}
	}
}

void APlayerCharacter::ModifyItemDrag()
{
	bModifyItemDrag = true;
}

void APlayerCharacter::StopModifyItemDrag()
{
	bModifyItemDrag = false;
}

