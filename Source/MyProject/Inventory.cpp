// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "PlayerCharacter.h"
#include "ItemBase.h"

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
	return InventorySlots.IsValidIndex(Index);
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
/**
 * This is the main to add any item to the players inventory
 */
bool AInventory::AddItem(TSubclassOf<AItemBase> Item, int32 Amount, int32& Rest)
{
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
			int32 LocalAmount = Amount;
			LocalAmount -= 1;
			if (LocalAmount > 0)
			{
				int32 Remainder = LocalAmount;
				AddItem(Item, LocalAmount, Remainder);
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
				AddItem(Item, LeftOver, RestAmount);
				return true;
			}
			// case that existing amount and amount to be added are not exceeding the aloowed stack size
			else
			{
				FInventorySlot ItemToAdd;
				ItemToAdd.ItemClass = Item;
				ItemToAdd.Amount = InventorySlots[Index].Amount + Amount;
				InventorySlots[Index] = ItemToAdd;
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
					AddItem(Item, LeftOver, RestAmount);
					return true;
				}
				else
				{
					ItemToAdd.Amount = Amount;
					InventorySlots[Index] = ItemToAdd;
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

int32 AInventory::GetAmountAtIndex(int32 Index)
{
	return InventorySlots[Index].Amount;
}



