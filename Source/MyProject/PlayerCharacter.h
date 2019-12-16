// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProjectCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API APlayerCharacter : public AMyProjectCharacter
{
	GENERATED_BODY()

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:

	void Interact();
	void DoesNotInteract();

	bool bIsInteracting;
};
