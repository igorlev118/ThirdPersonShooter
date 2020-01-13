// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardKeys")
	struct FBlackboardKeySelector IndexKey;

	UPROPERTY(EditAnywhere, Category = "BlackboardKeys")
	struct FBlackboardKeySelector WaypointKey;
};
