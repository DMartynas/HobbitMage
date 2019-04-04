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
	orcHealth = 200;
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
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) > 210.f && Attack == true) Attack = false;
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation())));
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) < 250.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Boool")));
					Controller->StopMovement();
					Walk = false;
					Attack = true;
				}
				if (FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()) > 200.f && Attack == false && Walk == false)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Walk")));
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation())));
					UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FVector::Distance(Controller->GetPawn()->GetActorLocation(), Pawn->PlayerCamera->GetComponentLocation()));
					Controller->StopMovement();
					MoveToPlayer();
				}
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
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Moving")));
	AAIController* Controller = Cast<AAIController>(GetController());
	if (Controller)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			AMagePawn* Pawn = Cast<AMagePawn>(PC->GetPawn());
			if (Pawn)
			{
				Controller->StopMovement();
				Walk = true;
				Controller->SetFocus(Pawn);
				//Controller->MoveToLocation(Pawn->PlayerCamera->GetComponentLocation(), 100.0F);
				Controller->MoveToActor(Pawn, 80);
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
	OnOrcDied.Broadcast();
	OnOrcKilled();
}
void AOrcCharacter::DecreaseHealth()
{
	if (orcHealth > 0)
	{
		orcHealth -= meeleDamage;
		if (orcHealth <= 0) KillOrc();
	}
}

