// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupFactory.h"
#include "Pickup.h"
#include "Engine/World.h"
#include "ItemBase.h"

// Sets default values
APickupFactory::APickupFactory() : Super()
{

}

// Called when the game starts or when spawned
void APickupFactory::BeginPlay()
{
	Super::BeginPlay();
	if (!InitOnStart) { return; }
	
	SpawnPickupActor();
}

void APickupFactory::SpawnPickupActor()
{
	if (MinAmount > MaxAmount)
	{
		MinAmount = 1;
	}

	FVector SpawnPos = GetActorLocation();
	FRotator SpawnRot = GetActorRotation();
	APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupActor, SpawnPos, SpawnRot);
	Pickup->Amount = FMath::RandRange(MinAmount, MaxAmount);
	const int32 RandomItemPicker = FMath::RandRange(0, ItemPool.Num() - 1);
	Pickup->ItemToAdd = ItemPool[RandomItemPicker];
	Pickup->DatabaseKey = ItemPool[RandomItemPicker].GetDefaultObject()->DatabaseID;
}


