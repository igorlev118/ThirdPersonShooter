// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomPickupFactory.h"
#include  "ItemBase.h"

ARandomPickupFactory::ARandomPickupFactory() : Super()
{
}

void ARandomPickupFactory::BeginPlay()
{
	if (MinAmount > MaxAmount)
	{
		MinAmount = 1;
	}
	Amount = FMath::RandRange(MinAmount, MaxAmount);
	const int32 RandomItemPicker = FMath::RandRange(0, ItemPool.Num() - 1);
	ItemToAdd = ItemPool[RandomItemPicker];
	DatabaseKey = ItemToAdd.GetDefaultObject()->DatabaseID;
}
