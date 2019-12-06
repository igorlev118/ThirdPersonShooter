// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "MyProjectGameMode.generated.h"

// Menu Enums / Structs

/**Used to identify the current menu state and decide which widget should be shown
 * and which ones can be destroyed
 */
UENUM(BlueprintType)
enum class EWidgetSelector : uint8
{
	WS_NONE UMETA(DisplayName = "None"),	
	WS_PAUSE UMETA(DisplayName = "Pause"),
	WS_MAINMENU UMETA(DisplayName = "Main Menu"),
	WS_LOADING UMETA(DisplayName = "Loading")
};

/**Used in the options menu to manage the different settings sub menus*/
UENUM(BlueprintType)
enum class ESettingsSelector : uint8
{
	WS_NONE UMETA(DisplayName = "None"),
	WS_GAPHICS UMETA(DisplayName = "Graphics"),
	WS_SOUND UMETA(DisplayName = "Sound"),
	WS_CONTROLS UMETA(DisplayName = "Controls")
};

/**Used to identify the setting of a certain graphics settings category*/
UENUM(BlueprintType)
enum class EGraphicsPresets : uint8
{
	EG_HIGH UMETA(DisplayName = "High"),
	EG_MEDIUM UMETA(DisplayName = "Medium"),
	EG_LOW UMETA(DisplayName = "Low")
};

/**Holds the different graphics settings categories used in the menu*/
UENUM(BlueprintType)
enum class EGraphicsCategory : uint8
{
	GC_ANTIALIASING UMETA(DisplayName = "Anti Aliasing"),
	GC_POSTPROCESSING UMETA(DisplayName = "Post Processing"),
	GC_SHADOWS UMETA(DisplayName = "Shadows"),
	GC_TEXTURES UMETA(DisplayName = "Textures"),
	GC_EFFECTS UMETA(DisplayName = "Effects")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDeath, APawn*, Player, AActor*, Enemy);

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()	

	UPROPERTY()
	TArray<AActor*> CorpseList;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> EnemyList;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxAllowedCorpses = 5;

public:

	AMyProjectGameMode();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*>& GetEnemyList();

	FOnEnemyDeath OnEnemyDeath;

	void RegisterToCorpseList(AActor* Corpse, APawn* Killer);

	void RegisterToEnemyList(AActor* Enemy);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* ItemDataBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBehaviorTreeComponent* MultiAIBT;
};



