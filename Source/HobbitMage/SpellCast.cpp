// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellCast.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MagePawn.h"

// Sets default values
ASpellCast::ASpellCast(const FObjectInitializer &ObjInitializer)
{
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;
	OverlapSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));
	OverlapSphere->SetGenerateOverlapEvents(true);

	SpellCastParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpellCastParticles"));
	SpellCastParticles->SetupAttachment(OverlapSphere);

}

// Called when the game starts or when spawned
void ASpellCast::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpellCast::BeginSpellCastSphereOverlap);
}

void ASpellCast::CastSpell(AMagePawn* Mage, const FHitResult &Hit)
{
}

void ASpellCast::BeginSpellCastSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMagePawn* Pawn = Cast<AMagePawn>(OtherActor);
	if (Pawn)
	{
		CastSpell(Pawn, SweepResult);
	}
}

