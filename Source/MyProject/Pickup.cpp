// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "CustomCharacterController.h"
#include "Inventory.h"

// Sets default values
APickup::APickup()
{
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(SphereCollider);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapse);
}

void APickup::OnOverlapse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && OtherActor && OtherActor != this && OtherComp)
	{
		
		if (Player->bIsInteracting)
		{
			UE_LOG(LogTemp, Warning, TEXT("report"))
			ACustomCharacterController* Controller = Cast<ACustomCharacterController>(Player->GetController());
			if (Controller->InventoryReference)
			{
				int32 PotentialRest = 0;
				bool bIsItemAdded = Controller->InventoryReference->AddItem(ItemToAdd, Amount, PotentialRest);

				if (bIsItemAdded && PotentialRest > 0)
				{
					Amount = PotentialRest;
				}
				else
				{
					Destroy();
				}
			}
		}
		
	}
}

