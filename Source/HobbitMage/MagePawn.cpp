// Fill out your copyright notice in the Description page of Project Settings.

#include "MagePawn.h"
#include "MagicBeing.h"
#include "Components/SceneComponent.h"
#include "HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "SpellDetector.h"
#include "Particles/ParticleSystemComponent.h"
#include "SpellCast.h"
#include <algorithm>
#include "SpellDetector.h"
#include "Components/AudioComponent.h"
#include "Engine.h"
#include "HobbitMageGameModeBase.h"

// Sets default values
AMagePawn::AMagePawn(const FObjectInitializer &ObjInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	StaffController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("StaffController"));
	StaffController->SetupAttachment(RootComponent);
	StaffController->SetTrackingMotionSource(FName("Special_1"));

	StaffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaffMesh"));
	StaffMesh->SetupAttachment(StaffController);
	StaffMesh->SetCollisionProfileName("Staff");
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->SetupAttachment(StaffMesh);

	CircleSpellParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CircleSpellParticle"));
	CircleSpellParticle->SetupAttachment(StaffMesh);

	ShallNotPassParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShallNotPassParticle"));
	ShallNotPassParticle->SetupAttachment(StaffMesh);
	ShallNotPassParticle->bAutoActivate = false;

	YouShallNot = CreateDefaultSubobject<UAudioComponent>(TEXT("YouShallNot"));
	YouShallNot->SetupAttachment(PlayerCamera);
	YouShallNot->bAutoActivate = false;

	Pass = CreateDefaultSubobject<UAudioComponent>(TEXT("Pass"));
	Pass->SetupAttachment(PlayerCamera);
	Pass->bAutoActivate = false;

	SpellCastCooldown = 1.5F;
	bReadyToCast = true;
	bCastingShallNotPass = false;

	MaxBufferedPositions = 19;
	PositionRegisteringRate = 1.0F / 15.0F;

	CircleAcceptanceChance = 0.8F;
	RadiusVariation = 0.2F;
	IsCircle = false;

	StaffVelocityBufferSize = 8;
	CurrentVelocityBufferIndex = 0;

	CircleChance = 0.0F;

	TriangleAcceptanceChance = 5;
	AngleCounter = 0;
	resetTime = true;

	spellCasted = false;

	timeStarted;

	OutslashVector1 = FVector::ZeroVector;
	OutslashVector2 = FVector::ZeroVector;
	FVector circlePos = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AMagePawn::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle_RegisterPointTimer, this, &AMagePawn::RegisterPoint, PositionRegisteringRate, true);
	}
	LastLocation = StaffController->GetComponentLocation();
}

void AMagePawn::RegisterPoint()
{
	if (!spellCasted)
	{
		FVector NewPosition = StaffController->GetComponentLocation();
		if (BufferedPositions.Num() == MaxBufferedPositions)
		{
			BufferedPositions.RemoveAt(0);
		}
		if (BufferedPositions.Num() > 0)
			if(FVector2D::Distance(FVector2D(BufferedPositions[BufferedPositions.Num() - 1].X, BufferedPositions[BufferedPositions.Num() - 1].Z), FVector2D(NewPosition.X, NewPosition.Z)) > 2.f)
				BufferedPositions.Add(NewPosition);
		if(BufferedPositions.Num() < 1)
			BufferedPositions.Add(NewPosition);
		DetectSpell();
	}
}

void AMagePawn::DetectSpell()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle handle;

		float CircleRadius = 0.0F;
		//FVector* first;
		//FVector* second;
		//FSpellDetector::DynamicTimeWarping();
		//debugMessage = GetWorld()->GetMapName();
		if ("UEDPIE_0_MountaiMap" == GetWorld()->GetMapName())
		{
			int attackType = FSpellDetector::DetectSlashOrStab(BufferedPositions, this, OutslashVector1, OutslashVector2);
			if (attackType == 1)
				changeWeaponToHammer  = true;
			else if (attackType == 2)
				changeWeaponToSword = true;
		}
		else if ("UEDPIE_0_DetailMap" == GetWorld()->GetMapName())
		{
			//FSpellDetector::RecordMovement(BufferedPositions, this, OutslashVector1, OutslashVector2);
			if (FSpellDetector::DetectCircle(BufferedPositions, CircleAcceptanceChance, RadiusVariation, CirclePosition, CircleRadius, this))
			{
				BufferedPositions.Empty();
				IsCircle = false;
				spellCasted = true;
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("--------------------CIRCLE")));
				resetTime = true;
				AngleCounter = 0;
				UnreadySpellCast();
				World->GetTimerManager().SetTimer(TimerHandle_SpellCastCooldown, this, &AMagePawn::SpellCastReady, SpellCastCooldown);
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(this->PlayerCamera->GetComponentLocation() + this->PlayerCamera->GetForwardVector() * 100);
				SpawnTransform.SetRotation(PlayerCamera->GetComponentQuat());
				ASpellCast* Cast = World->SpawnActor<ASpellCast>(CircleSpellCastClass, SpawnTransform);
				if (Cast)
				{
					Cast->SpellCastParticles->SetFloatParameter("CircleRadius", CircleRadius);
					BufferedPositions.Empty();
				}
			}
			else if (FSpellDetector::DetectTriangle(BufferedPositions, TrianglePosition, AngleCounter, resetTime, timeStarted, this))
			{
				BufferedPositions.Empty();
				spellCasted = true;
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("--------------------Triangle")));
				UnreadySpellCast();
				World->GetTimerManager().SetTimer(TimerHandle_SpellCastCooldown, this, &AMagePawn::SpellCastReady, SpellCastCooldown);
				FTransform SpawnTransform;

				SpawnTransform.SetLocation(this->PlayerCamera->GetComponentLocation() + this->PlayerCamera->GetForwardVector() * 100);
				SpawnTransform.SetRotation(PlayerCamera->GetComponentQuat());
				ASpellCast* Cast = World->SpawnActor<ASpellCast>(TriangleSpellCastClass, SpawnTransform);
				if (Cast)
				{
					Cast->SpellCastParticles->SetFloatParameter("CircleRadius", CircleRadius);

				}
			}

			/*else if (FSpellDetector::DetectLightning(BufferedPositions, TrianglePosition, AngleCounter, resetTime, timeStarted, this))
			{
				spellCasted = true;
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("--------------------LIGHTNING")));
				UnreadySpellCast();
				World->GetTimerManager().SetTimer(TimerHandle_SpellCastCooldown, this, &AMagePawn::SpellCastReady, SpellCastCooldown);
				FTransform SpawnTransform;

				SpawnTransform.SetLocation(CirclePosition);
				SpawnTransform.SetRotation(PlayerCamera->GetComponentQuat());
				ASpellCast* Cast = World->SpawnActor<ASpellCast>(CircleSpellCastClass, SpawnTransform);
				if (Cast)
				{
					Cast->SpellCastParticles->SetFloatParameter("CircleRadius", CircleRadius);
				}
			}*/

			/*else if (FSpellDetector::DetectTriangle(BufferedPositions, TrianglePosition, AngleCounter, resetTime, timeStarted, this))
			{
				spellCasted = true;
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("--------------------Triangle")));
				UnreadySpellCast();
				World->GetTimerManager().SetTimer(TimerHandle_SpellCastCooldown, this, &AMagePawn::SpellCastReady, SpellCastCooldown);
				FTransform SpawnTransform;
				//TrianglePosition -= AMagePawn::PlayerCamera->GetForwardVector();

				SpawnTransform.SetLocation(TrianglePosition);
				SpawnTransform.SetRotation(PlayerCamera->GetComponentQuat());
				AMagicBeing* Cast = World->SpawnActor<AMagicBeing>(MagicBeingSpellCastClass, SpawnTransform);

				if (Cast)
				{

					//Cast->
					//Cast->SpellCastParticles->SetFloatParameter("CircleRadius", CircleRadius);
				}
			}*/

			else
			{
				AHobbitMageGameModeBase* GameMode = Cast<AHobbitMageGameModeBase>(GetWorld()->GetAuthGameMode());
				if (GameMode)
				{
					if (FSpellDetector::DetectShallNotPass(BufferedPositions, PlayerCamera->GetComponentLocation(), 86.0F) && !GameMode->bShallNotPassUsed)
					{
						UnreadySpellCast();
						ShallNotPassParticle->Activate();
						bCastingShallNotPass = true;
						YouShallNot->Play();
					}
				}
			}
		}
	}
}

TArray<FVector2D> AMagePawn::ReadFromFile(FString FileName)
{
	FString SaveDirectory = FString("C:/Users/VR-Lab-06/Desktop");
	bool AllowOverwriting = true;
	FString TextToRead = "";
	TArray<FVector2D> output;
	TArray<FString> vectors;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	float X = -999999.f;
	float Y = -999999.f;
	float Z = -999999.f;
	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting)
		{

			FFileHelper::LoadFileToString(TextToRead, *AbsoluteFilePath);
		}
		output = AMagePawn::callFunction(TextToRead);


	return output;
}

void AMagePawn::writeToFile(FString TextToSave, FString FileName)
{
	FString SaveDirectory = FString("C:/Users/VR-Lab-06/Desktop");
	bool AllowOverwriting = true;


	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting)
		{
			FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
	}
}

void AMagePawn::UnreadySpellCast()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_RegisterPointTimer);
	bReadyToCast = false;
	TrailParticle->DeactivateSystem();
}

void AMagePawn::SpellCastReady()
{
	bReadyToCast = true;
	TrailParticle->ActivateSystem();
	BufferedPositions.Empty();
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle_RegisterPointTimer, this, &AMagePawn::RegisterPoint, PositionRegisteringRate, true);
	}
}

// Called every frame
void AMagePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Delta = StaffController->GetComponentLocation() - LastLocation;
	FVector CurrentVelocity = Delta / DeltaTime;
	if (StaffVelocityBuffer.Num() < MaxBufferedPositions)
	{
		StaffVelocityBuffer.Add(CurrentVelocity);
	}
	else
	{
		StaffVelocityBuffer[CurrentVelocityBufferIndex++] = CurrentVelocity;
		if (CurrentVelocityBufferIndex >= StaffVelocityBufferSize)
		{
			CurrentVelocityBufferIndex = 0;
		}
	}

	FVector CurrentSmoothedVelocity = GetStaffVelocity();

	if (bCastingShallNotPass)
	{
		CircleChance = CurrentSmoothedVelocity.Size();

		FVector LastVelDir = LastVelocity;
		LastVelDir.Normalize();

		if (LastVelocity.Size() > 10.0F && CurrentSmoothedVelocity.Size() < 10.0F && FVector::DotProduct(LastVelDir, -FVector::UpVector) > 0.8F)
		{
			bCastingShallNotPass = false;
			ShallNotPassParticle->Deactivate();
			SpellCastReady();
			YouShallNot->Stop();
			Pass->Play();
		}
	}

	LastLocation = StaffController->GetComponentLocation();
	LastVelocity = CurrentSmoothedVelocity;
}

// Called to bind functionality to input
void AMagePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AMagePawn::GetStaffVelocity()
{
	FVector OutVelocity = FVector::ZeroVector;
	if (StaffVelocityBuffer.Num() > 0)
	{
		for (int i = 0; i < StaffVelocityBuffer.Num(); i++)
		{
			OutVelocity += StaffVelocityBuffer[i];
		}
		OutVelocity /= StaffVelocityBuffer.Num();
	}
	return OutVelocity;
}
void AMagePawn::onTimerEnd()
{

}

