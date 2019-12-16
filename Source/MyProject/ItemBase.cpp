// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"
#include "Engine/World.h"
#include "Inventory.h"

// Sets default values
AItemBase::AItemBase() : Super()
{ 		
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::OnUse_Implementation()
{
	if (Inventory->RemoveItemAtIndex(Index, 1))
	{
		if (ItemInfo.Category != EItemCategory::IC_QUESTITEM)
		{
			Destroy();
		}		
	}	
}


