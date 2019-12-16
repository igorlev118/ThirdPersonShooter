// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomCharacterController.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot : public FTableRowBase
{
	GENERATED_BODY()

	FInventorySlot()
	{
		Amount = 0;
		ItemClass = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Amount;
};

UCLASS()
class MYPROJECT_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Prooperties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AmountOfSlots = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 MaxStackSize = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bShowObtainedItem;

	// Internal methods
	UFUNCTION(BlueprintCallable)
		bool AddItemInsideClass(TSubclassOf<AItemBase> Item, FName DatabaseKey, int32 Amount, int32& Rest);

public:


	// Events

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateSpecificSlot(int32 Index);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool ShowObtainedItem(TSubclassOf<AItemBase> Item, int32 Amount);
	
	UFUNCTION(BlueprintImplementableEvent)
	bool AddItem(TSubclassOf<AItemBase> Item, FName DatabaseKey, int32 Amount, int32& Rest);

	// Functions

	/**
	 * Returns true if the chosen index does not contain an item
	 */
	UFUNCTION(BlueprintCallable)
	bool IsSLotEmpty(int32 Index);

	/**
	 * Returns all necessary information for further operation of an item at a chosen index
	 * Returns true if an item was found
	 */
	UFUNCTION(BlueprintCallable)
	bool GetItemInfoAtIndex(int32 Index, int32& ItemAmount, FItemInfo& ItemInfo);

	/**
	 * Finds the next empty slot in the inventory container
	 */
	bool SearchEmptySlot(int32& Index);	

	/**
	 * Checks whether there is a free stack of items of a certain amount and returns the success and the index
	 */
	bool SearchFreeStack(int32& Index, TSubclassOf<AItemBase> Item);	

	/**
	* Can delete an item at a given position with a certain amount
	*/
	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int32 Index, int32 Amount);

	/**
	* Returns the amount of items at given index
	*/
	UFUNCTION(BlueprintCallable)
	int32 GetAmountAtIndex(int32 Index);

	/**
	 * Swaps two inventory slots.
	 * Is protected against false index inputs
	 */
	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 IndexA, int32 IndexB);

	/**
	* Can split a stack of stackable items, does nothing if there is no stack or if the item is not stackable
	*/
	UFUNCTION(BlueprintCallable)
	bool SplitStack (int32 Index, int32 Amount);

	/**
	* Calls the on use function of the item at a given slot. Note that the use is determined by the item
	*/
	UFUNCTION(BlueprintCallable)
	void UsingItemAtSlot(int32 Index);

	/**
	 * Can be used to add the amount of one item to the amount of another one
	 * will only return success if both items are of the same class
	 */
	UFUNCTION(BlueprintCallable)
	bool AddToIndex(int32 IndexFrom, int32 IndexTo);

	/**
	 * Can be used to split the amount of a stack-able item
	 * to another empty slot
	 */
	UFUNCTION(BlueprintCallable)
	bool SplitStackToIndex(int32 IndexFrom, int32 IndexTo, int32 Amount);	
};
