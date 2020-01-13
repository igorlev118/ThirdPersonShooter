// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"
#include "Perception/AIPerceptionComponent.h"


ACustomAIController::ACustomAIController() : Super()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void ACustomAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ACustomAIController::OnPerception);
	if (!ensure(BlackboardData) || !ensure(BehaviorTree)) { return; }
	UseBlackboard(BlackboardData, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
	Owner = Cast<AAICharacter>(GetPawn());
}

void ACustomAIController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{	
	if (Actor->ActorHasTag(PlayerTag))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(EnemyKey, Actor);
		}
		else
		{
			BlackboardComponent->ClearValue(EnemyKey);
		}
	}	
}
