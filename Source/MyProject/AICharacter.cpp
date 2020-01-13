// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Weapon->SetupAttachment(GetMesh());

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetMesh());	
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

