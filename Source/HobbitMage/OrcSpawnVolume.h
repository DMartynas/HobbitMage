// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrcSpawnVolume.generated.h"

UCLASS()
class HOBBITMAGE_API AOrcSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrcSpawnVolume(const FObjectInitializer &ObjInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Spawn Volume")
	class UBoxComponent* BoxVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	FVector GetRandomPointInVolume();

};
