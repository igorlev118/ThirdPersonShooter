// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

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
		if (Clip <= 0) { Reload(); return; }

		bCanFire = false;
		Clip--;
		FHitResult OutHit;
		FVector Start = OwningPlayer->GetAimCamera()->GetComponentLocation();
		FVector End = Start + OwningPlayer->GetAimCamera()->GetForwardVector() * Range; // actual range the weapon can shoot
		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
		UGameplayStatics::PlaySound2D(this, FireSound);
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Miss!")); }		
		float FireDuration = OwningPlayer->PlayAnimMontage(FireAnim);
		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetAnim, FireDuration, true);		
	}	

	// Auto Fire Mode

	else if (!Stats.bHasBurstFireMode && Stats.bHasFullAutoMode)
	{
		if (!OwningPlayer || !bCanFire) { return; }
		if (Clip <= 0) { Reload(); return; }				
		
		Clip--;
		FHitResult OutHit;
		FVector Start = OwningPlayer->GetAimCamera()->GetComponentLocation();
		FVector End = Start + OwningPlayer->GetAimCamera()->GetForwardVector() * Range; // actual range the weapon can shoot
		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
		UGameplayStatics::PlaySound2D(this, FireSound);
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Miss!")); }
		float PlayRate = FireAnim->GetPlayLength();		
		float TargetPlayRate = 1 / Stats.FireRate / PlayRate;		
		float FireDuration = OwningPlayer->PlayAnimMontage(FireAnim, TargetPlayRate);
		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetAnim, TargetPlayRate, true);
	}
}

void AWeaponBase::StopFire()
{
	if (Stats.bHasFullAutoMode)
	{
		bCanFire = false;
		GetWorld()->GetTimerManager().SetTimer(ShotStopper, this, &AWeaponBase::DelayHandler, 0.2f, true);
	}
}
	

void AWeaponBase::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	bCanFire = false;
	if (ReserveAmmo <= 0)	{return;}
	if (ReserveAmmo <= Clip) { Clip = ReserveAmmo; return; }
	Clip = Stats.MagazineSize;
	ReserveAmmo -= Stats.MagazineSize;

	float ReloadDuration = OwningPlayer->PlayAnimMontage(ReloadAnim);
	GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::DelayHandler, ReloadDuration, true);
}

void AWeaponBase::DelayHandler()
{
	bCanFire = true;
	GetWorld()->GetTimerManager().ClearTimer(ShotStopper);
}

void AWeaponBase::ResetAnim()
{		
	OwningPlayer->StopAnimMontage();
	GetWorld()->GetTimerManager().ClearTimer(Delay);
	if (Stats.bHasFullAutoMode)
	{
		StartFire();
	}
	bCanFire = true;
}
