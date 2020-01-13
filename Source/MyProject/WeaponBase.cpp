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

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
			if (Stats.OnHitParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Stats.OnHitParticle, OutHit.Location);
			}			
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

		DrawDebugLine(GetWorld(), this->GetActorLocation(), End, FColor::Red, false, 2, 0, 12.333);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
			if (Stats.OnHitParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Stats.OnHitParticle, OutHit.Location);
			}
		}
		else { UE_LOG(LogTemp, Warning, TEXT("Miss!")); }
		float PlayRate = FireAnim->GetPlayLength();		
		float TargetPlayRate = 1 / Stats.FireRate / PlayRate;		
		float FireDuration = OwningPlayer->PlayAnimMontage(FireAnim, TargetPlayRate);

		GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::ResetAnim, FireDuration, true);
	}
}

void AWeaponBase::StopFire()
{
	if (Stats.bHasFullAutoMode)
	{
		bCanFire = false;
		ShotCounter = 0;
		GetWorld()->GetTimerManager().SetTimer(ShotStopper, this, &AWeaponBase::DelayHandler, 0.2f);
	}
}
	

void AWeaponBase::Reload()
{
	if (Clip == Stats.MagazineSize) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	bCanFire = false;	
	ShotCounter = 0;
	if (ReserveAmmo <= 0)	{return;}
	if (ReserveAmmo + Clip <= Stats.MagazineSize) 
	{	Clip += ReserveAmmo; 
		ReserveAmmo = 0; 
	}
	else
	{
		ReserveAmmo -= Stats.MagazineSize - Clip;
		Clip = Stats.MagazineSize;
	}
	
	OwningPlayer->UpdateAmmoOnly();
	float ReloadDuration = OwningPlayer->PlayAnimMontage(ReloadAnim);
	GetWorld()->GetTimerManager().SetTimer(Delay, this, &AWeaponBase::DelayHandler, ReloadDuration);
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
