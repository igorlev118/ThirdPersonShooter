// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "CustomCharacterController.h"
#include "Pickup.h"
#include "Inventory.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "WeaponBase.h"


APlayerCharacter::APlayerCharacter() : Super()
{
	GetCharacterMovement()->MaxWalkSpeed = Walkspeed;
	Health = MaxHealth;	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// World Interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);

	// Inventory Controls
	PlayerInputComponent->BindAction("ItemDragModifier", IE_Pressed, this, &APlayerCharacter::ModifyItemDrag);
	PlayerInputComponent->BindAction("ItemDragModifier", IE_Released, this, &APlayerCharacter::StopModifyItemDrag);

	// Movement
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::NoSprint);

	// Combat
	PlayerInputComponent->BindAction("EquipWeapon", IE_Pressed, this, &APlayerCharacter::EquipAttachedWeapon);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{	
	if (bIsSprinting)
	{
		float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
		float TargetSpeed = FMath::FInterpTo(CurrentSpeed, Sprintspeed, GetWorld()->DeltaTimeSeconds, InterpolationSpeed);
		GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
	}
	else
	{
		float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
		float TargetSpeed = FMath::FInterpTo(CurrentSpeed, Walkspeed, GetWorld()->DeltaTimeSeconds, InterpolationSpeed);
		GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
	}	
}


void APlayerCharacter::Sprint()
{
	bIsSprinting = true;
	
}
	

void APlayerCharacter::NoSprint()
{
	bIsSprinting = false;
	
}

void APlayerCharacter::ResetEquipWeapon()
{
	bIsAnimationPlaying = false;
	GetWorld()->GetTimerManager().ClearTimer(AnimationPlaytime);
}


void APlayerCharacter::EquipAttachedWeapon()
{
	if (!MainWeapon && !SecondaryWeapon || bIsAnimationPlaying) { return; }
	bIsAnimationPlaying = true;
	float AnimTime = PlayAnimMontage(EquipWeaponAnim);
	GetWorld()->GetTimerManager().SetTimer(AnimationPlaytime, this, &APlayerCharacter::ResetEquipWeapon, AnimTime, true);
}

void APlayerCharacter::EquipAttachedWeaponForAnim()
{
	if (!CurrentlyEquippedWeapon)
	{
		if (MainWeapon)
		{
			bIsMainWeaponEquipped = true;
			bIsSecondaryWeaponEquipped = false;
			CurrentlyEquippedWeapon = MainWeapon;
			MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("EquippedWeapon"));
			return;
		}
		if (SecondaryWeapon)
		{
			bIsMainWeaponEquipped = false;
			bIsSecondaryWeaponEquipped = true;
			CurrentlyEquippedWeapon = SecondaryWeapon;
			SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("EquippedWeapon"));
			return;
		}
	}
	else
	{
		if (CurrentlyEquippedWeapon == MainWeapon)
		{			
			bIsMainWeaponEquipped = false;
			CurrentlyEquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("MainWeapon"));
			if (SecondaryWeapon)
			{				
				bIsSecondaryWeaponEquipped = true;
				CurrentlyEquippedWeapon = SecondaryWeapon;
				SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("EquippedWeapon"));
				return;
			}
			CurrentlyEquippedWeapon = nullptr;
			return;
		}
		if (CurrentlyEquippedWeapon == SecondaryWeapon)
		{
			bIsSecondaryWeaponEquipped = false;
			CurrentlyEquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Secondaryweapon"));
			if (MainWeapon)
			{
				bIsMainWeaponEquipped = true;				
				CurrentlyEquippedWeapon = MainWeapon;
				MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("EquippedWeapon"));
				return;
			}
			CurrentlyEquippedWeapon = nullptr;
			return;
		}
	}	


}

void APlayerCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	switch (Weapon->Stats.Category)
	{
	case EWeaponCategory::WC_PRIMARY:

		if (MainWeapon)
		{
			ACustomCharacterController* Controller = Cast<ACustomCharacterController>(GetController());
			int32 Rest;
			Controller->InventoryReference->AddItem(MainWeapon->InventoryItem, MainWeapon->DatabaseID, 1, Rest);
			Controller->InventoryReference->AddItem
			if (CurrentlyEquippedWeapon == MainWeapon)
			{
				CurrentlyEquippedWeapon = nullptr;
			}
			MainWeapon->Destroy();
		}
				
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("MainWeapon"));
		MainWeapon = Weapon;
		bIsMainWeaponEquipped = false;		
		break;

	case EWeaponCategory::WC_SECONDARY:

		if (SecondaryWeapon)
		{
			ACustomCharacterController* Controller = Cast<ACustomCharacterController>(GetController());
			int32 Rest;
			Controller->InventoryReference->AddItem(SecondaryWeapon->InventoryItem, SecondaryWeapon->DatabaseID, 1, Rest);
			if (CurrentlyEquippedWeapon == SecondaryWeapon)
			{
				CurrentlyEquippedWeapon = nullptr;
			}
			SecondaryWeapon->Destroy();
		}

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Secondaryweapon"));
		SecondaryWeapon = Weapon;		
		bIsSecondaryWeaponEquipped = false;
		break;

	default:
		break;
	}
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

