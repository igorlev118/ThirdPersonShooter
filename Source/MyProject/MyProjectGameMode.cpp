// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "Player/MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"


AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/ThirdPersonCPP/Blueprints/PlayerCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class; 
	}
}

TArray<AActor*>& AMyProjectGameMode::GetEnemyList()
{
	return EnemyList;
}

void AMyProjectGameMode::RegisterToCorpseList(AActor* Corpse, APawn* Killer)
{
	Corpse->SetActorTickEnabled(false);

	CorpseList.Add(Corpse);
	OnEnemyDeath.Broadcast(Killer, Corpse);

	/*int32 EnemyListIndex = EnemyList.Find(Corpse);
	if (EnemyListIndex != INDEX_NONE)
	{
		EnemyList.RemoveAt(EnemyListIndex);
	}

	if (CorpseList.Num() > MaxAllowedCorpses)
	{
		AActor* ActorToDestroy = CorpseList[0];
		CorpseList.RemoveAt(0);
		if (ActorToDestroy)
		{
			ActorToDestroy->Destroy();
			ActorToDestroy = nullptr;			
		}
	}	*/
}

void AMyProjectGameMode::RegisterToEnemyList(AActor* Enemy)
{
	EnemyList.Add(Enemy);
}
