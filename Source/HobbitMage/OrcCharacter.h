// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OrcCharacter.generated.h"

UCLASS()
class HOBBITMAGE_API AOrcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOrcCharacter(const FObjectInitializer &ObjInitializer);

public:

	UPROPERTY(EditAnywhere, Category = "Orc")
	bool bWalkToTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Orc")
	void OnOrcKilled();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToPlayer();

	void KillOrc();

};
