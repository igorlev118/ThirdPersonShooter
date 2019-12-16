// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "CustomCharacterController.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	IC_NONE UMETA(DisplayName = "This is only a default"),
	IC_CONSUMABLE UMETA(DisplayName = "Consumable"),
	IC_WEAPON UMETA(DisplayName = "Weapon"),
	IC_ARMOR UMETA(DisplayName = "Armor"),
	IC_QUESTITEM UMETA(DisplayName = "Quest Item")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	IR_NONE UMETA(DisplayName = "This is only a default"),
	IR_COMMON UMETA(DisplayName = "Common"),
	IR_RARE  UMETA(DisplayName = "Rare"),
	IR_EPIC UMETA(DisplayName = "Epic"),
	IR_LEGENDARY UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FItemInfo()
	{
		Name = FText::FromString(TEXT("Choose a name"));
		Description = FText::FromString(TEXT("Enter description"));
		Use = FText::FromString(TEXT("Enter an Item usage"));
		Icon = nullptr;
		Category = EItemCategory::IC_NONE;
		Rarity = EItemRarity::IR_NONE;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemRarity Rarity;

};

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class AInventory> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AInventory* InventoryReference;
};
