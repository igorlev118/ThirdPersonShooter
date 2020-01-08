// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

void UWeaponComponent::BeginPlay()
{
	if (USkeletalMeshComponent* MainParent = Cast<USkeletalMeshComponent>(GetAttachParent()))
	{
		SetMasterPoseComponent(MainParent);
	}
}
