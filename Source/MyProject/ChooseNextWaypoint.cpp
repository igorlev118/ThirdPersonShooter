// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CustomAIController.h"
#include "AICharacter.h"
#include "PatrolRouteComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// cache needed components to get the patrol route component
	ACustomAIController* AIController = Cast<ACustomAIController>(OwnerComp.GetAIOwner());	
	AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());	
	UPatrolRouteComponent* PatrolRoute = AICharacter->FindComponentByClass<UPatrolRouteComponent>();	
	if (!ensure(PatrolRoute)) { UE_LOG(LogTemp, Warning, TEXT("No Patrol Route Component")) return EBTNodeResult::Failed; }

	// save found waypoints in local container and check if there are any
	TArray<AActor*> PatrolPoints = PatrolRoute->GetWaypoints();
	if (PatrolPoints.Num() == 0) { UE_LOG(LogTemp, Warning, TEXT("No Valid Route")) return EBTNodeResult::Failed; }

	// get the current value of the owners indexkey selector and set the waypoint key to
	// the patrolpoint at the index position  
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	if(PatrolPoints.Num() != 1)
	{
		BlackboardComp->SetValueAsObject(FirstWaypointKey.SelectedKeyName, PatrolPoints[0]);
		BlackboardComp->SetValueAsObject(LastWaypointKey.SelectedKeyName, PatrolPoints[PatrolPoints.Num() - 1]);
	}
	
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// incrementing index, while ensuring array stays in bound and setting the index key
	// to a new value for repeating the proceedure
	Index++;
	Index %= PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index); 

	return EBTNodeResult::Succeeded;
}
