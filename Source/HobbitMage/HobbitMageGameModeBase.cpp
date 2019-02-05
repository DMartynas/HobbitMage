// Fill out your copyright notice in the Description page of Project Settings.

#include "HobbitMageGameModeBase.h"
#include "OrcSpawnVolume.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "OrcCharacter.h"

AHobbitMageGameModeBase::AHobbitMageGameModeBase(const FObjectInitializer &ObjInitializer)
{
	Score = 0;
	bGameStarted = false;
	bShallNotPassUsed = false;
}

void AHobbitMageGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AOrcSpawnVolume> Itr(GetWorld()); Itr; ++Itr)
	{
		SpawnVolumes.Add(*Itr);
	}
}

void AHobbitMageGameModeBase::SpawnOrc()
{
	int RandVolume = FMath::RandRange(0, SpawnVolumes.Num() - 1);
	FVector UrukLocation = SpawnVolumes[RandVolume]->GetRandomPointInVolume();

	FTransform Transform;
	Transform.SetLocation(UrukLocation);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AOrcCharacter>(OrcClass, Transform, Params);

	float CurrentGameTime = GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle_GameTime);

	float NormalizedGameTime = CurrentGameTime / GameDuration;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnTime, this, &AHobbitMageGameModeBase::SpawnOrc, FMath::Lerp<float>(SpawnRate, MinSpawnRate, NormalizedGameTime));
}

void AHobbitMageGameModeBase::StartGame()
{
	if (!bGameStarted)
	{
		bGameStarted = true;
		bShallNotPassUsed = false;
		Score = 0;
		OnScoreAdded.Broadcast(0);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_GameTime, this, &AHobbitMageGameModeBase::StopGame, GameDuration);
		SpawnOrc();
	}
}

void AHobbitMageGameModeBase::StopGame()
{
	if (bGameStarted)
	{
		bGameStarted = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SpawnTime);
		for (TActorIterator<AOrcCharacter> Itr(GetWorld()); Itr; ++Itr)
		{
			(*Itr)->KillOrc();
		}
	}
}

void AHobbitMageGameModeBase::AddScore(int InScore)
{
	if (bGameStarted)
	{
		Score += InScore;
		OnScoreAdded.Broadcast(Score);
	}
}
