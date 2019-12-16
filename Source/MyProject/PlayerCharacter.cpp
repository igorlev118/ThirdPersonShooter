// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::DoesNotInteract);
}

void APlayerCharacter::Interact()
{	
	bIsInteracting = true;
	UE_LOG(LogTemp, Warning, TEXT("%i"), bIsInteracting)
}

void APlayerCharacter::DoesNotInteract()
{	
	bIsInteracting = false;
	UE_LOG(LogTemp, Warning, TEXT("%i"), bIsInteracting)
}
