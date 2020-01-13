// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CustomAIController.h"
#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tasks/AITask_MoveTo.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "VisualLogger/VisualLogger.h"


EBTNodeResult::Type UWalkTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	FBTMoveToTaskMemory* MyMemory = reinterpret_cast<FBTMoveToTaskMemory*>(NodeMemory);

	ACustomAIController* AIController = Cast<ACustomAIController>(OwnerComp.GetAIOwner());
	AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacter->GetMovementComponent());
	if (!ensure(MovementComponent)) { return EBTNodeResult::Failed; }

	MovementComponent->MaxWalkSpeed = WalkSpeed;
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AActor* Waypoint = Cast<AActor>(BlackboardComponent->GetValueAsObject(WaypointKey.SelectedKeyName));
	if (!ensure(Waypoint)) { return EBTNodeResult::Failed; }		
	
	FPathFollowingRequestResult RequestResult = AIController->MoveTo(Waypoint);	
	if (RequestResult.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path found"))
		MyMemory->MoveRequestID = RequestResult.MoveId;
		WaitForMessage(OwnerComp, UBrainComponent::AIMessage_MoveFinished, RequestResult.MoveId);
		WaitForMessage(OwnerComp, UBrainComponent::AIMessage_RepathFailed);

		NodeResult = EBTNodeResult::InProgress;
	}
	else if (RequestResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		NodeResult = EBTNodeResult::Succeeded;
	}

	return NodeResult;	
}

void UWalkTo::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID,
	bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Message Received"))
	Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
	bSuccess &= (Message != UBrainComponent::AIMessage_RepathFailed);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}	
}

