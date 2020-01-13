// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"

AWeaponBase::AWeaponBase()
{
	Appearance = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	Appearance->SetupAttachment(RootComponent);	
}

void AWeaponBase::StartFire()
{
	// Single Fire Mode
	if (!Stats.bHasBurstFireMode && !Stats.bHasFullAutoMode)
	{
		if (!OwningPlayer || !bCanFire) { return; }
		bCanFire = false;
		float FireDuration = OwningPlayer->PlayAnimMontage(FireAnim);
		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetFire, FireDuration, true);		
	}

	// Burst Fire Mode

	// Auto Fire Mode
}

void AWeaponBase::StopFire()
{
}

void AWeaponBase::ResetFire()
{
	bCanFire = true;
	GetWorld()->GetTimerManager().ClearTimer(Delay);
}
