// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"

AWeaponBase::AWeaponBase() : Super()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Appearance = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	Appearance->SetupAttachment(RootComponent);	
}

void AWeaponBase::StartFire()
{
	if (bIsReloading || bIsShooting) { return; }
	bCanFire = true;

	// Single Fire Mode
	if (!Stats.bHasBurstFireMode && !Stats.bHasFullAutoMode)
	{		
		if (!OwningPlayer || !bCanFire) { return; }
		if (Clip <= 0) { Reload(); return; }

		bIsShooting = true;
		bCanFire = false;
		Clip--;
		OwningPlayer->UpdateAmmoOnly();
		OwningPlayer->ManageCrosshairShoot();

		FHitResult OutHit;
		FVector Start = OwningPlayer->GetAimCamera()->GetComponentLocation();
		FVector End;
		FVector EndPoint = Start + OwningPlayer->GetAimCamera()->GetForwardVector() * Range;// actual range the weapon can shoot in a straight line
		if (OwningPlayer->GetVelocity().Size() > 0)
		{

			End = FVector(EndPoint.X + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread),
						  EndPoint.Y + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread),
						  EndPoint.Z + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread));
		}
		else
		{
			End = EndPoint;
		}

		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
		UGameplayStatics::PlaySound2D(this, FireSound);
		UGameplayStatics::SpawnEmitterAttached(Stats.MuzzleFlash, Appearance, TEXT("MuzzleFlash"), 
						Appearance->GetSocketLocation(TEXT("MuzzleFlash")), Appearance->GetSocketRotation(TEXT("MuzzleFlash")), EAttachLocation::KeepWorldPosition);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
			if (Stats.OnHitParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Stats.OnHitParticle, OutHit.Location);
			}

			FVector MuzzleSocketLocation = Appearance->GetSocketLocation(TEXT("MuzzleFlash"));
			FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, OutHit.Location);
			GetWorld()->SpawnActor(Stats.Projectile, &MuzzleSocketLocation, &ProjectileRotation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Miss!"));
			FVector MuzzleSocketLocation = Appearance->GetSocketLocation(TEXT("MuzzleFlash"));
			FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, End);
			GetWorld()->SpawnActor(Stats.Projectile, &MuzzleSocketLocation, &ProjectileRotation);
		}

		if(FireAnim)
		{
			float FireDuration = OwningPlayer->PlayAnimMontage(FireAnim);
		}

		float TargetPlayRate = 1 / Stats.FireRate;
		

		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetAnim, TargetPlayRate, true);		
	}	

	// Auto Fire Mode
	else if (!Stats.bHasBurstFireMode && Stats.bHasFullAutoMode && bCanFire)
	{
		if (!OwningPlayer) { return; }
		if (Clip <= 0) { Reload(); return; }				

		bIsShooting = true;
		Clip--;
		OwningPlayer->UpdateAmmoOnly();
		OwningPlayer->ManageCrosshairShoot();
		ShotCounter++;

		FHitResult OutHit;
		FVector Start = OwningPlayer->GetAimCamera()->GetComponentLocation();
		FVector End; 
		FVector EndPoint = Start + OwningPlayer->GetAimCamera()->GetForwardVector() * Range;// actual range the weapon can shoot in a straight line
		if (OwningPlayer->GetVelocity().Size() > 0 || ShotCounter >= 4)
		{
			
			End = FVector(EndPoint.X + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread),
						  EndPoint.Y + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread),
						  EndPoint.Z + FMath::RandRange(-this->Stats.MaxSpread, this->Stats.MaxSpread));
		}
		else if(ShotCounter < 4)
		{
			End = Start + OwningPlayer->GetAimCamera()->GetForwardVector() * Range;
		}
		
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
		UGameplayStatics::PlaySound2D(this, FireSound);
		UGameplayStatics::SpawnEmitterAttached(Stats.MuzzleFlash, Appearance, TEXT("MuzzleFlash"), 
						Appearance->GetSocketLocation(TEXT("MuzzleFlash")), Appearance->GetSocketRotation(TEXT("MuzzleFlash")), EAttachLocation::KeepWorldPosition);

		//DrawDebugLine(GetWorld(), this->GetActorLocation(), End, FColor::Red, false, 2, 0, 12.333);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
			if (Stats.OnHitParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Stats.OnHitParticle, OutHit.Location);
			}

			FVector MuzzleSocketLocation = Appearance->GetSocketLocation(TEXT("MuzzleFlash"));
			FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, OutHit.Location);			
			GetWorld()->SpawnActor(Stats.Projectile, &MuzzleSocketLocation, &ProjectileRotation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Miss!"));
			FVector MuzzleSocketLocation = Appearance->GetSocketLocation(TEXT("MuzzleFlash"));
			FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, End);
			GetWorld()->SpawnActor(Stats.Projectile, &MuzzleSocketLocation, &ProjectileRotation);
		}
				
		float TargetPlayRate = 1 / Stats.FireRate;		
		OwningPlayer->PlayAnimMontage(FireAnim, 10);
		
		GetWorld()->GetTimerManager().ClearTimer(ResetShots);
		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetAnim, TargetPlayRate, true);		
	}
}

void AWeaponBase::StopFire()
{		
	if (Stats.bHasFullAutoMode && bCanFire)
	{
		bCanFire = false;
		GetWorld()->GetTimerManager().SetTimer(ResetShots, this, &AWeaponBase::ResetShotCounter, 0.5);
	}	
}


void AWeaponBase::ReloadHandler()
{
	bCanFire = true;
	bIsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}

void AWeaponBase::ResetShotCounter()
{
	ShotCounter = 0;
}

void AWeaponBase::ResetAnim()
{	
	bIsShooting = false;
	GetWorld()->GetTimerManager().ClearTimer(Delay);	
	if (Stats.bHasFullAutoMode && bCanFire)
	{		
		StartFire();		
	}
	else
	{
		bCanFire = true;		
	}	
}

void AWeaponBase::Reload()
{
	if (Clip == Stats.MagazineSize) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	bCanFire = false;
	bIsReloading = true;
	ShotCounter = 0;
	if (ReserveAmmo <= 0) { return; }
	if (ReserveAmmo + Clip <= Stats.MagazineSize)
	{
		Clip += ReserveAmmo;
		ReserveAmmo = 0;
	}
	else
	{
		ReserveAmmo -= Stats.MagazineSize - Clip;
		Clip = Stats.MagazineSize;
	}
	float ReloadDuration = OwningPlayer->PlayAnimMontage(ReloadAnim);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::ReloadHandler, ReloadDuration);
	OwningPlayer->UpdateAmmoOnly();
}
