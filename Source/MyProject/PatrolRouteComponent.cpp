// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolRouteComponent.h"

// Sets default values for this component's properties
UPatrolRouteComponent::UPatrolRouteComponent()
{

	
}

TArray<AActor*> UPatrolRouteComponent::GetWaypoints() const
{
	return Waypoints;
}


