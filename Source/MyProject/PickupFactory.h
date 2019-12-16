// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupFactory.generated.h"

UCLASS()
class MYPROJECT_API APickupFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APickup> PickupActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InitOnStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmount;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AItemBase>> ItemPool;

	UFUNCTION(BlueprintCallable)
	void SpawnPickupActor();
};
