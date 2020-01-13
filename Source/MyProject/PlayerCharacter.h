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
	FTimerHandle AnimationPlaytime;

	// Private Methods
	void ResetEquipWeapon();

	// to set animation based on equipment
	bool bIsMainWeaponEquipped;
	bool bIsSecondaryWeaponEquipped;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Walkspeed = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Sprintspeed = 600;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InterpolationSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon")
	class AWeaponBase* MainWeapon;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* SecondaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* CurrentlyEquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* EquipWeaponAnim;

public:

	// Methods
	void Interact();
	void ModifyItemDrag();
	void StopModifyItemDrag();
	void Sprint();
	void NoSprint();
	void EquipAttachedWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipAttachedWeaponForAnim();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(class AWeaponBase* Weapon);

	// getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetModifyItemDrag() { return bModifyItemDrag; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSprinting() { return bIsSprinting; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsMainWeaponEquipped() { return bIsMainWeaponEquipped; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSecondaryWeaponEquipped() { return bIsSecondaryWeaponEquipped; }

	
	
	// public properties
	class APickup* FoundPickup = nullptr;	
};
