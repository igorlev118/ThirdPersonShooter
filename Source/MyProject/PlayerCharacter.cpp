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
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"


APlayerCharacter::APlayerCharacter() : Super()
{
	GetCharacterMovement()->MaxWalkSpeed = Walkspeed;
	Health = MaxHealth;

	AimCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimCameraArm"));
	AimCameraArm->TargetArmLength = 10;
	AimCameraArm->SetupAttachment(RootComponent);
		
	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCamera"));	
	AimCamera->SetupAttachment(AimCameraArm);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
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
	PlayerInputComponent->BindAction("TakeAim", IE_Pressed, this, &APlayerCharacter::Takeaim);
	PlayerInputComponent->BindAction("LoseAim", IE_Released, this, &APlayerCharacter::LoseAim);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::Crouch);	
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

	if (bIsAiming)
	{
		if (this->GetVelocity().Size() > 0 || CurrentlyEquippedWeapon->GetShotCounter() >= 4)
		{
			CrosshairStyle(2);
		}
		else
		{
			CrosshairStyle(1);
		}
	}	
}


void APlayerCharacter::Sprint()
{
	if (bIsAiming) { return; }
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

void APlayerCharacter::BeginPlay() 
{	
	Super::BeginPlay();
	InitialRotation = GetActorRotation();
}


void APlayerCharacter::EquipAttachedWeapon()
{
	if (!MainWeapon && !SecondaryWeapon || bIsAnimationPlaying || bIsAiming) { return; }
	bIsAnimationPlaying = true;
	float AnimTime = PlayAnimMontage(EquipWeaponAnim);
	GetWorld()->GetTimerManager().SetTimer(AnimationPlaytime, this, &APlayerCharacter::ResetEquipWeapon, AnimTime, true);
}

void APlayerCharacter::Takeaim()
{
	if (!CurrentlyEquippedWeapon) { return; }
	GetFollowCamera()->SetActive(false, true);	
	AimCamera->SetActive(true);	
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;	
	bIsAiming = true;
	CurrentlyEquippedWeapon->bCanFire = true;
	CurrentlyEquippedWeapon->bIsShooting = false;
	ManageCrosshair();
}

void APlayerCharacter::LoseAim()
{
	if (!CurrentlyEquippedWeapon || !bIsAiming) { return; }
	GetFollowCamera()->SetActive(true);
	AimCamera->SetActive(false, true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	SetActorRotation(FRotator(InitialRotation.Pitch, GetActorRotation().Yaw, InitialRotation.Roll));
	CurrentlyEquippedWeapon->StopFire();
	CurrentlyEquippedWeapon->bIsShooting = false;
	bIsAiming = false;
	ManageCrosshair();
}

void APlayerCharacter::Fire()
{
	if (CurrentlyEquippedWeapon && bIsAiming)
	{		
		CurrentlyEquippedWeapon->StartFire();	
	}
}

void APlayerCharacter::StopFire()
{
	if (CurrentlyEquippedWeapon && bIsAiming)
	{		
		CurrentlyEquippedWeapon->StopFire();		
	}
}

void APlayerCharacter::Reload()
{
	if (CurrentlyEquippedWeapon)
	{		
		CurrentlyEquippedWeapon->Reload();
	}
}

void APlayerCharacter::Crouch()
{
	if (bIsAiming) { return; }
	if (!bIsCrouched)
	{
		Super::Crouch();
		bIsCrouched = true;
	}
	else
	{
		Super::UnCrouch();
		bIsCrouched = false;
	}
	
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
			Controller->InventoryReference->AddItemInsideClass(MainWeapon->InventoryItem, MainWeapon->DatabaseID, 1, Rest);			
			if (CurrentlyEquippedWeapon == MainWeapon)
			{
				CurrentlyEquippedWeapon = nullptr;
			}
			MainWeapon->Destroy();			
		}
				
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("MainWeapon"));
		MainWeapon = Weapon;
		bIsMainWeaponEquipped = false;
		UpdateWeapons();
		break;

	case EWeaponCategory::WC_SECONDARY:

		if (SecondaryWeapon)
		{
			ACustomCharacterController* Controller = Cast<ACustomCharacterController>(GetController());
			int32 Rest;
			Controller->InventoryReference->AddItemInsideClass(SecondaryWeapon->InventoryItem, SecondaryWeapon->DatabaseID, 1, Rest);
			if (CurrentlyEquippedWeapon == SecondaryWeapon)
			{
				CurrentlyEquippedWeapon = nullptr;
			}
			SecondaryWeapon->Destroy();
		}

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Secondaryweapon"));
		SecondaryWeapon = Weapon;		
		bIsSecondaryWeaponEquipped = false;
		UpdateWeapons();
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

