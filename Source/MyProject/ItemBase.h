// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterController.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"


UCLASS()
class MYPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInventory* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DatabaseID;

	FItemInfo ItemInfo;

	UFUNCTION(BlueprintNativeEvent)
	void OnUse();
};
