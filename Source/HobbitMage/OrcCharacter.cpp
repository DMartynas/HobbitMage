// Fill out your copyright notice in the Description page of Project Settings.

#include "OrcCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "MagePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "HobbitMageGameModeBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

// Sets default values
AOrcCharacter::AOrcCharacter(const FObjectInitializer &ObjInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	orcHealth = 100;
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
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AAIController* Controller = Cast<AAIController>(GetController());
	if (Controller)
	{
		if (PC)
		{
			AMagePawn* Pawn = Cast<AMagePawn>(PC->GetPawn());
			if (Pawn)
			{
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) > 300.f && Attack == false)
				{
					Controller->StopMovement();
					Walk = true;
					Controller->SetFocus(Pawn);
					//Controller->MoveToLocation(Pawn->PlayerCamera->GetComponentLocation(), 100.0F);
					Controller->MoveToActor(Pawn, 200);
				}
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) < 300.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Stopped")));
					Controller->StopMovement();
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation())));
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) < 300.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Boool")));
					Controller->StopMovement();
					Walk = false;
					Attack = true;
				}
				else if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) > 400.f) Attack = false;
			}
		}
	}

}

// Called to bind functionality to input
void AOrcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOrcCharacter::MoveToPlayer()
{
	/*if (bWalkToTarget)
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
					
				}
			}
		}
	}*/
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
void AOrcCharacter::DecreaseHealth()
{
	if (orcHealth <= 0) KillOrc();
	else 
	{
		orcHealth -= meeleDamage;
		if (orcHealth <= 0) KillOrc();
	}
}

