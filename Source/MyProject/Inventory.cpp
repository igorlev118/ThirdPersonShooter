// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "PlayerCharacter.h"
#include "ItemBase.h"
#include "MyProjectGameMode.h"
#include "Engine/World.h"

// Sets default values
AInventory::AInventory()
{
	
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.SetNum(AmountOfSlots);
}

bool AInventory::IsSLotEmpty(int32 Index)
{	
	return !IsValid(InventorySlots[Index].ItemClass);
}

bool AInventory::GetItemInfoAtIndex(int32 Index, int32& ItemAmount, FItemInfo& ItemInfo)
{
	if(TSubclassOf<AItemBase> Item = InventorySlots[Index].ItemClass)
	{
		ItemAmount = InventorySlots[Index].Amount;
		ItemInfo = Item.GetDefaultObject()->ItemInfo;
		return true;
	}
	return false;
}

bool AInventory::SearchEmptySlot(int32& Index)
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (!IsValid(InventorySlots[i].ItemClass))
		{
			Index = i; // found empty slot!
			return true;
		}
	}

	return false;
}

bool AInventory::SearchFreeStack(int32& Index, TSubclassOf<AItemBase> Item)
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (IsValid(InventorySlots[i].ItemClass))
		{
			if (InventorySlots[i].ItemClass == Item && InventorySlots[i].Amount < MaxStackSize)
			{
				Index = i;
				return true;
			}
		}
	}

	return false;
}

bool AInventory::AddItem(TSubclassOf<AItemBase> Item, FName DatabaseKey, int32 Amount, int32& Rest)
{
	AMyProjectGameMode* GameMode = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* InitData = GameMode->ItemDataBase;
	FItemInfo* Setup = InitData->FindRow<FItemInfo>(DatabaseKey, TEXT(""));

	Item.GetDefaultObject()->ItemInfo = *Setup;

	// case that item is NOT stackable
	if (!Item.GetDefaultObject()->ItemInfo.bCanBeStacked)
	{
		int32 Index = 0;
		if (SearchEmptySlot(Index))
		{
			FInventorySlot ItemToAdd;
			ItemToAdd.ItemClass = Item;
			ItemToAdd.Amount = 1;
			InventorySlots[Index] = ItemToAdd;
			OnUpdateSpecificSlot(Index);
			int32 LocalAmount = Amount;
			LocalAmount -= 1;
			if (LocalAmount > 0)
			{
				int32 Remainder = LocalAmount;
				AddItem(Item, DatabaseKey, LocalAmount, Remainder);
				return true;
			}
			return true;
		}

		Rest = Amount;
		return false;
		
	}	

	// case for stackable Items
	else
	{
		int32 Index = 0;
		if (SearchFreeStack(Index, Item))
		{
			// case that the amount already inside the slot and the amount to added exceed the allowed stack size
			if (InventorySlots[Index].Amount + Amount > MaxStackSize)
			{
				FInventorySlot ItemToAdd;
				ItemToAdd.ItemClass = Item;
				ItemToAdd.Amount = MaxStackSize;
				int32 LeftOver = InventorySlots[Index].Amount + Amount - MaxStackSize;
				int32 RestAmount = InventorySlots[Index].Amount + Amount - MaxStackSize;
				InventorySlots[Index] = ItemToAdd;			
				OnUpdateSpecificSlot(Index);
				AddItem(Item, DatabaseKey, LeftOver, RestAmount);
				return true;
			}
			// case that existing amount and amount to be added are not exceeding the aloowed stack size
			else
			{
				FInventorySlot ItemToAdd;
				ItemToAdd.ItemClass = Item;
				ItemToAdd.Amount = InventorySlots[Index].Amount + Amount;
				InventorySlots[Index] = ItemToAdd;
				OnUpdateSpecificSlot(Index);
				return true;
			}
			
		}
		else
		{
			int32 Index = 0;
			if (SearchEmptySlot(Index))
			{
				FInventorySlot ItemToAdd;
				ItemToAdd.ItemClass = Item;

				// if amount to add is bigger than the alowed stack size, add the rest in a separate call
				if (Amount > MaxStackSize)
				{
					ItemToAdd.Amount = MaxStackSize;
					InventorySlots.Insert(ItemToAdd, Index);
					int32 LeftOver = MaxStackSize - Amount;
					int32 RestAmount = MaxStackSize - Amount;
					AddItem(Item, DatabaseKey, LeftOver, RestAmount);
					return true;
				}
				else
				{
					ItemToAdd.Amount = Amount;
					InventorySlots[Index] = ItemToAdd;
					OnUpdateSpecificSlot(Index);
					return true;
				}


			}
			else
			{
				Rest = Amount;
				return false; // Item cannot be added
			}

		}
		
	}
}


bool AInventory::RemoveItemAtIndex(int32 Index, int32 Amount)
{
	// When the slot is empty, there is no operation
	if (IsSLotEmpty(Index) && Amount < 1)
	{
		return false;
	}

	// When the amount to remove is higher 
	if (Amount >= GetAmountAtIndex(Index))
	{
		InventorySlots[Index].Amount = 0;
		InventorySlots[Index].ItemClass = nullptr;
		OnUpdateSpecificSlot(Index);
		return true;
	}

	// When the amount to remove is less than the amount in the slot, leave the rest value
	else
	{
		InventorySlots[Index].Amount -= Amount;
		OnUpdateSpecificSlot(Index);
		return true;
	}

	return false;
}


int32 AInventory::GetAmountAtIndex(int32 Index)
{
	return InventorySlots[Index].Amount;
}

bool AInventory::SwapSlots(int32 IndexA, int32 IndexB)
{
	// if any of the given indices is bigger than the length of the array it must be out of bound
	if (InventorySlots.Num() < IndexA || InventorySlots.Num() < IndexB)
	{
		return false;
	}

	// swap both slots
	FInventorySlot Temp = InventorySlots[IndexA];
	InventorySlots[IndexA] = InventorySlots[IndexB];
	InventorySlots[IndexB] = Temp;
	OnUpdateSpecificSlot(IndexA);
	OnUpdateSpecificSlot(IndexB);

	return true;
}


bool AInventory::SplitStack(int32 Index, int32 Amount)
{
	// if the slot is empty, the amount in the slot smaller than the existing amount or the item is not stackable
	// this means there can't be a split operation
	if (IsSLotEmpty(Index) || InventorySlots[Index].Amount <= Amount || !InventorySlots[Index].ItemClass.GetDefaultObject()->ItemInfo.bCanBeStacked)
	{
		return false;
	}

	int32 EmptyIndex;
	if (!SearchEmptySlot(EmptyIndex))
	{
		return false;
	}
	InventorySlots[EmptyIndex].ItemClass = InventorySlots[Index].ItemClass;
	InventorySlots[EmptyIndex].Amount = Amount;
	InventorySlots[Index].Amount -= Amount;
	OnUpdateSpecificSlot(Index);
	OnUpdateSpecificSlot(EmptyIndex);
	return true;
}


void AInventory::UsingItemAtSlot(int32 Index)
{
	if(IsValid(InventorySlots[Index].ItemClass))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(InventorySlots[Index].ItemClass, SpawnParameters);
		Item->Index = Index;
		Item->Inventory = this;
		Item->OnUse();
	}
}

bool AInventory::AddToIndex(int32 IndexFrom, int32 IndexTo)
{		
	if (InventorySlots[IndexFrom].ItemClass == InventorySlots[IndexTo].ItemClass && InventorySlots[IndexTo].Amount < MaxStackSize && InventorySlots[IndexFrom].ItemClass.GetDefaultObject()->ItemInfo.bCanBeStacked)
	{
		if (GetAmountAtIndex(IndexTo) + GetAmountAtIndex(IndexFrom) <= MaxStackSize)
		{
			InventorySlots[IndexTo].Amount = GetAmountAtIndex(IndexFrom) + GetAmountAtIndex(IndexTo);
			InventorySlots[IndexFrom].ItemClass = nullptr;
			InventorySlots[IndexFrom].Amount = 0;
			OnUpdateSpecificSlot(IndexFrom);
			OnUpdateSpecificSlot(IndexTo);
			return true;
		}
		else
		{
			InventorySlots[IndexFrom].Amount = GetAmountAtIndex(IndexFrom) - (MaxStackSize - GetAmountAtIndex(IndexTo));
			InventorySlots[IndexTo].Amount = MaxStackSize;
			OnUpdateSpecificSlot(IndexFrom);
			OnUpdateSpecificSlot(IndexTo);
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool AInventory::SplitStackToIndex(int32 IndexFrom, int32 IndexTo, int32 Amount)
{
	if (IsSLotEmpty(IndexTo) && !IsSLotEmpty(IndexFrom) 
		&& InventorySlots[IndexFrom].ItemClass.GetDefaultObject()->ItemInfo.bCanBeStacked 
		&& InventorySlots[IndexFrom].Amount > 1 && InventorySlots[IndexFrom].Amount > Amount)
	{
		InventorySlots[IndexFrom].Amount -= Amount;
		InventorySlots[IndexTo].Amount = Amount;
		InventorySlots[IndexTo].ItemClass = InventorySlots[IndexFrom].ItemClass;
		OnUpdateSpecificSlot(IndexFrom);
		OnUpdateSpecificSlot(IndexTo);
		return true;
	}
	else
	{
		return false;
	}
}



