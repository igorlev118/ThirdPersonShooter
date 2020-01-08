// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	Appearance = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	Appearance->SetupAttachment(RootComponent);	
}
