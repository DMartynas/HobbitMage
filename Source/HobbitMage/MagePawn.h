// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <ctime>
#include <chrono>
#include "MagePawn.generated.h"

using namespace std::chrono;

UCLASS()
class HOBBITMAGE_API AMagePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMagePawn(const FObjectInitializer &ObjInitializer);

public:

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	class UMotionControllerComponent* StaffController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	class UStaticMeshComponent* StaffMesh;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	class UParticleSystemComponent* TrailParticle;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	UParticleSystemComponent* CircleSpellParticle;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	UParticleSystemComponent* ShallNotPassParticle;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	class UAudioComponent* YouShallNot;

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	UAudioComponent* Pass;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCasts")
	float SpellCastCooldown;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCasts")
	TSubclassOf<class ASpellCast> CircleSpellCastClass;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | General")
	int MaxBufferedPositions;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | General")
	float PositionRegisteringRate;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Circle")
	float CircleAcceptanceChance;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Triangle")
	float TriangleAcceptanceChance;

	int AngleCounter;
	bool resetTime;

	system_clock::time_point timeStarted;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Circle")
	float RadiusVariation;

	UPROPERTY(EditAnywhere, Category = "Mage")
	int StaffVelocityBufferSize;

	UPROPERTY(BlueprintReadOnly, Category = "Mage")
	float CircleChance;

	bool spellCasted;

protected:

	TArray<FVector> BufferedPositions;

	TArray<FVector> StaffVelocityBuffer;
	FVector LastLocation;
	int CurrentVelocityBufferIndex;

	bool bReadyToCast;

	bool bCastingShallNotPass;

	UPROPERTY(BlueprintReadOnly, Category="Mage")
	FVector LastVelocity;

	FTimerHandle TimerHandle_RegisterPointTimer;
	FTimerHandle TimerHandle_SpellCastCooldown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RegisterPoint();

	void UnreadySpellCast();
	void SpellCastReady();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetStaffVelocity();
};
