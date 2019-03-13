// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MagicBeing.generated.h"

UCLASS()
class HOBBITMAGE_API AMagicBeing : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	AMagicBeing(const FObjectInitializer &ObjInitializer);

public:

	UPROPERTY(EditAnywhere, Category = "MagicBeing")
		bool bWalkToTarget;



	UPROPERTY(BlueprintReadWrite, Category = "MagicBeing")
		int meeleDamage;

protected:
	AMagicBeing();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "MagicBeing")
		void KillBeing();



};
