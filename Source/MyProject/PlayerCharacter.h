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

		APlayerCharacter();

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

private:	
	
	bool bModifyItemDrag;
	bool bIsSprinting;
	bool bIsAnimationPlaying = false;
	bool bIsAiming;
	FTimerHandle AnimationPlaytime;	

	// Private Methods
	void ResetEquipWeapon();

	// to set animation
	bool bIsMainWeaponEquipped;
	bool bIsSecondaryWeaponEquipped;
	bool bIsCrouched;


protected:

	

	void BeginPlay() override;

	// Aim Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* AimCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* AimCameraArm;

	// Movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Walkspeed = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Sprintspeed = 600;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InterpolationSpeed = 1;


	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon")
	class AWeaponBase* MainWeapon;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* SecondaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentlyEquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* EquipWeaponAnim;


	// Setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float GatheredExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float NeededExpForNextLevel = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FRotator InitialRotation;

public:

	// BP Events
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHealthBar();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateLevel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateProgress();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateWeapons();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateAmmoOnly();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ManageCrosshair();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ManageCrosshairShoot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CrosshairStyle(int32 Index);

	// Methods
	void Interact();
	void ModifyItemDrag();
	void StopModifyItemDrag();
	void Sprint();
	void NoSprint();
	void EquipAttachedWeapon();
	void Takeaim();
	void LoseAim();
	void Fire();
	void StopFire();
	void Reload();
	void Crouch();	

	UFUNCTION(BlueprintCallable)
	void EquipAttachedWeaponForAnim();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(class AWeaponBase* Weapon);

	// getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetModifyItemDrag() const { return bModifyItemDrag; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsMainWeaponEquipped() const { return bIsMainWeaponEquipped; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSecondaryWeaponEquipped() const { return bIsSecondaryWeaponEquipped; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetLevel() const { return Level; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetExp() const { return GatheredExp; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetNeededExp() const { return NeededExpForNextLevel; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsCrouched() const { return bIsCrouched; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetAimCamera() const { return AimCamera; }
	
	// public properties
	class APickup* FoundPickup = nullptr;	
};
