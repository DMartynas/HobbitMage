// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HobbitMageGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreAddedDelegate, int, NewScore);

UCLASS()
class HOBBITMAGE_API AHobbitMageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHobbitMageGameModeBase(const FObjectInitializer &ObjInitializer);

public:

	UPROPERTY(EditAnywhere, Category = "Hobbit Mage")
	float SpawnRate = 5.0F;

	UPROPERTY(EditAnywhere, Category = "Hobbit Mage")
	float MinSpawnRate = 0.25F;

	UPROPERTY(EditAnywhere, Category = "Hobbit Mage")
	float GameDuration = 90.0F;

	UPROPERTY(EditAnywhere, Category = "Hobbit Mage")
	TSubclassOf<class AOrcCharacter> OrcClass;

	bool bShallNotPassUsed;

protected:
	TArray<class AOrcSpawnVolume*> SpawnVolumes;

	int Score;

	bool bGameStarted;

	FTimerHandle TimerHandle_GameTime;
	FTimerHandle TimerHandle_SpawnTime;

protected:

	void BeginPlay();

	void SpawnOrc();

public:

	UFUNCTION(BlueprintCallable, Category = "Hobbit Mage")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Hobbit Mage")
	void StopGame();

	void AddScore(int InScore);

	UPROPERTY(BlueprintAssignable, Category = "Hobbit Mage")
	FScoreAddedDelegate OnScoreAdded;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hobbit Mage")
	FORCEINLINE bool IsGameStarted() { return bGameStarted; }
};
