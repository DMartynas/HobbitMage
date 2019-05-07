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

	UFUNCTION()
		void onTimerEnd();

	UFUNCTION(BlueprintImplementableEvent)
		TArray<FVector2D> callFunction(const FString& input);

	UFUNCTION(BlueprintCallable)
		void writeToFile(FString TextToSave, FString FileName);

	UFUNCTION(BlueprintCallable)
		TArray<FVector2D> ReadFromFile(FString FileName);

	
public:

	UPROPERTY(VisibleAnywhere, Category = "Mage")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	class UMotionControllerComponent* StaffController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	class UStaticMeshComponent* StaffMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	bool IsCircle;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCasts")
	TSubclassOf<class ASpellCast> CircleSpellCastClass;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCasts")
	TSubclassOf<class ASpellCast> TriangleSpellCastClass;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCasts")
	TSubclassOf<class AMagicBeing> MagicBeingSpellCastClass;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | General")
	int MaxBufferedPositions;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | General")
	float PositionRegisteringRate;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Circle")
	float CircleAcceptanceChance;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Triangle")
	float TriangleAcceptanceChance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	FString debugMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	bool changeWeaponToSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	bool callDetectCircleFunc = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	bool changeWeaponToHammer;

	int AngleCounter;
	bool resetTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	FVector CirclePosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mage")
	FVector TrianglePosition;

	system_clock::time_point timeStarted;

	UPROPERTY(EditAnywhere, Category = "Mage | SpellCast Detection | Circle")
	float RadiusVariation;

	UPROPERTY(EditAnywhere, Category = "Mage")
	int StaffVelocityBufferSize;

	UPROPERTY(BlueprintReadOnly, Category = "Mage")
	float CircleChance;

	bool spellCasted;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	TArray<FVector> BufferedPositions;

	TArray<FVector> StaffVelocityBuffer;
	FVector LastLocation;
	int CurrentVelocityBufferIndex;

	bool bReadyToCast;

	bool bCastingShallNotPass;

	FVector OutslashVector1;
	FVector OutslashVector2;

	UPROPERTY(BlueprintReadOnly, Category="Mage")
	FVector LastVelocity;

	FTimerHandle TimerHandle_RegisterPointTimer;
	FTimerHandle TimerHandle_SpellCastCooldown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RegisterPoint();
	void DetectSpell();
	void UnreadySpellCast();
	void SpellCastReady();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetStaffVelocity();
};
