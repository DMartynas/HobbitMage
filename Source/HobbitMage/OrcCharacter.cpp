// Fill out your copyright notice in the Description page of Project Settings.

#include "OrcCharacter.h"
#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "MagePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "HobbitMageGameModeBase.h"

// Sets default values
AOrcCharacter::AOrcCharacter(const FObjectInitializer &ObjInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

}

// Called when the game starts or when spawned
void AOrcCharacter::BeginPlay()
{
	Super::BeginPlay();
	MoveToPlayer();
}

// Called every frame
void AOrcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOrcCharacter::MoveToPlayer()
{
	if (bWalkToTarget)
	{
		AAIController* Controller = Cast<AAIController>(GetController());
		if (Controller)
		{
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				AMagePawn* Pawn = Cast<AMagePawn>(PC->GetPawn());
				if (Pawn)
				{
					Controller->SetFocus(Pawn);
					Controller->MoveToLocation(Pawn->PlayerCamera->GetComponentLocation(), 50.0F);
				}
			}
		}
	}
}

void AOrcCharacter::KillOrc()
{
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	AHobbitMageGameModeBase* GameMode = Cast<AHobbitMageGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->AddScore(10);
	}
	OnOrcKilled();
}

