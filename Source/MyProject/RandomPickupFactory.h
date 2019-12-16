// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "RandomPickupFactory.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ARandomPickupFactory : public APickup
{
	GENERATED_BODY()

protected:

	ARandomPickupFactory();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DropOnlyOneItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AItemBase>> ItemPool;
	
};
