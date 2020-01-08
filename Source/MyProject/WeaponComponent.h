// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeaponBase* Weapon;	
};
