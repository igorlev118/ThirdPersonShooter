// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CustomAIController.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

	ACustomAIController();

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	UBehaviorTree* BehaviorTree;	

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	FName PlayerTag = "Player";

	UPROPERTY(EditAnywhere, Category = "Behavior")
	UAIPerceptionComponent* AIPerception;

	class UBlackboardComponent* BlackboardComponent;
	class AAICharacter* Owner;
	FName EnemyKey;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);
};
