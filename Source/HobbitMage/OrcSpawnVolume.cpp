// Fill out your copyright notice in the Description page of Project Settings.

#include "OrcSpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AOrcSpawnVolume::AOrcSpawnVolume(const FObjectInitializer &ObjInitializer)
{
	BoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	BoxVolume->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AOrcSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AOrcSpawnVolume::GetRandomPointInVolume()
{
	return FMath::RandPointInBox(BoxVolume->Bounds.GetBox());
}
