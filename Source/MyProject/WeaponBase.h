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
		FireRate = 0;
		bHasFullAutoMode = false;
		bHasBurstFireMode = false;		
		MinSpread = 0;
		MaxSpread = 0;
		MagazineSize = 30;
		Category = EWeaponCategory::WC_NONE;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamagePerBullet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasFullAutoMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasBurstFireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxSpread;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponCategory Category;
};

UCLASS()
class MYPROJECT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

	AWeaponBase();

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WeaponDataBaseID;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWeaponStats Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USkeletalMeshComponent* Appearance;
};
