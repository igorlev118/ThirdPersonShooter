// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	WC_NONE UMETA(DisplayName = "This is only a default value!"),
	WC_PRIMARY UMETA(DisplayName = "Primary"),
	WC_SECONDARY UMETA(DisplayName = "Secondary")
};

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponStats()
	{
		DamagePerBullet = 0;
		HeadShotMultiplier = 2;
		FireRate = 0;
		bHasFullAutoMode = false;
		bHasBurstFireMode = false;				
		MaxSpread = 0;
		Recoil = 0;
		MagazineSize = 0;
		MaxAmmo = 0;
		Category = EWeaponCategory::WC_NONE;
		UIDisplay = nullptr;
		OnHitParticle = nullptr;
		MuzzleFlash = nullptr;
		Projectile = nullptr;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamagePerBullet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HeadShotMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasFullAutoMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasBurstFireMode;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Recoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponCategory Category;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* UIDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* OnHitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AProjectile> Projectile;
};

UCLASS()
class MYPROJECT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

	AWeaponBase();
	FTimerHandle Delay;
	FTimerHandle ReloadTimer;
	FTimerHandle ResetShots;
	int8 ShotCounter = 0;	

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class APlayerCharacter* OwningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class UAnimMontage* FireAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<AItemBase> InventoryItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	FName WeaponDataBaseID;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FWeaponStats Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 Clip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 ReserveAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 Range = 20000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	class USkeletalMeshComponent* Appearance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	USceneComponent* Root;

	bool bCanFire = true;
	bool bIsReloading;
	bool bIsShooting;

	// Methods	
	void StartFire();
	void StopFire();
	void Reload();
	void ReloadHandler();
	void ResetShotCounter();
	void ResetAnim();
	
	FORCEINLINE uint8 GetShotCounter() { return ShotCounter; }
};
