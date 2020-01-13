// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class MYPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float MaxHealth = 200;

	float CurrentHealth;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

};
