// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicBeing.h"
#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "MagePawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "HobbitMageGameModeBase.h"

// Sets default values
AMagicBeing::AMagicBeing(const FObjectInitializer &ObjInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AMagicBeing::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMagicBeing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMagicBeing::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMagicBeing::KillBeing()
{
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
}

