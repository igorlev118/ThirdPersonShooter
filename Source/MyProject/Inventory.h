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

public:

	// Events

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateSpecificSlot(int32 Index);	

	// Functions

	UFUNCTION(BlueprintCallable)
	bool IsSLotEmpty(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool GetItemInfoAtIndex(int32 Index, int32& ItemAmount, FItemInfo& ItemInfo);
		
	bool SearchEmptySlot(int32& Index);	
	
	bool SearchFreeStack(int32& Index, TSubclassOf<AItemBase> Item);

	UFUNCTION(BlueprintCallable)
	bool AddItem(TSubclassOf<AItemBase> Item, FName DatabaseKey, int32 Amount, int32& Rest);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int32 Index, int32 Amount);

	UFUNCTION(BlueprintCallable)
	int32 GetAmountAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 IndexA, int32 IndexB);

	UFUNCTION(BlueprintCallable)
	bool SplitStack (int32 Index, int32 Amount);

	UFUNCTION(BlueprintCallable)
	void UsingItemAtSlot(int32 Index);
};
