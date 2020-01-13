// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "PatrolRouteComponent.h"

// Sets default values
AAICharacter::AAICharacter() : Super()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Weapon->SetupAttachment(GetMesh());

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetMesh());

	PatrolRoute = CreateDefaultSubobject<UPatrolRouteComponent>(TEXT("PatrolRoute"));
	
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	HealthBuffer = MaxHealth;
	HealthBarWidget->SetVisibility(false);
}

void AAICharacter::ResetWidgetTimer()
{
	HealthBarWidget->SetVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(WidgetTimerHandle);
}

void AAICharacter::ITakeDamage(FName Bone, float Damage, float HeadShotMultiplier)
{
	if (Bone == TEXT("Head"))
	{
		CurrentHealth -= Damage * HeadShotMultiplier;
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage * HeadShotMultiplier)
	}
	else
	{
		CurrentHealth -= Damage;
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage)
	}

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}

	if (!HealthBarWidget->IsVisible())
	{
		HealthBarWidget->SetVisibility(true);
	}
	GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &AAICharacter::ResetWidgetTimer, WidgetTimer);
}

bool AAICharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealthBuffer != CurrentHealth && CurrentHealth > 0)
		HealthBuffer = FMath::FInterpConstantTo(HealthBuffer, CurrentHealth, GetWorld()->GetDeltaSeconds(), 30);
	else if (CurrentHealth <= 0)
		HealthBuffer = 0;
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

