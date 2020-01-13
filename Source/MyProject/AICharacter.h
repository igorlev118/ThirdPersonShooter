// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ITakeDamage.h"
#include "AICharacter.generated.h"

UCLASS()
class MYPROJECT_API AAICharacter : public ACharacter, public IITakeDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:

	// Methods
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ResetWidgetTimer();

	// Attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPatrolRouteComponent* PatrolRoute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float MaxHealth = 200;

	/*
	 * Time in seconds when the HP widget gets hidden after the last taken damage
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float WidgetTimer = 15;

	float CurrentHealth;

	float HealthBuffer;

	FTimerHandle WidgetTimerHandle;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ITakeDamage(FName Bone, float Damage, float HeadShotMultiplier) override;	

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthBuffer() const { return HealthBuffer; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
};
