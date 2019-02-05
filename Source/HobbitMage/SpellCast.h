// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellCast.generated.h"

UCLASS()
class HOBBITMAGE_API ASpellCast : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellCast(const FObjectInitializer &ObjInitializer);

public:

	UPROPERTY(VisibleAnywhere, Category = "Spell Cast")
	class USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, Category = "Spell Cast")
	class UParticleSystemComponent* SpellCastParticles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CastSpell(class AMagePawn* Mage, const FHitResult &Hit);

public:	

	UFUNCTION()
	void BeginSpellCastSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
