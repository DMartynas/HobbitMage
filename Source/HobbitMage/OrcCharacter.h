// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OrcCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOrcDiedDelegate);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orc")
	int orcHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Orc")
	int meeleDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Orc")
	void OnOrcKilled();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack)
	bool Attack = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack)
	bool Walk = false;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Orc")
	void KillOrc();

	UFUNCTION(BlueprintCallable, Category = "Orc")
	void DecreaseHealth();

	FOrcDiedDelegate OnOrcDied;

};
