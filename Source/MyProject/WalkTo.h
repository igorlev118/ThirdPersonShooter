// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WalkTo.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWalkTo : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
		
protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardKeys")
	struct FBlackboardKeySelector WaypointKey;

	UPROPERTY(EditAnywhere, Category = "BlackboardKeys")
	float WalkSpeed = 150;
};

