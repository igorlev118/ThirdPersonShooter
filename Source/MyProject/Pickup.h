// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MYPROJECT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carried Item")
	FName DatabaseKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carried Item")
	TSubclassOf<class AItemBase> ItemToAdd;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carried Item")
	int32 Amount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	class USphereComponent* SphereCollider;

	UFUNCTION()
	void OnOverlapse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

	UFUNCTION()
	void EndOverlapse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

