// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicMissile.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "OrcCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMagicMissile::AMagicMissile(const FObjectInitializer &ObjInitializer)
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetCollisionProfileName("OverlapAllDynamic");
	//SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetNotifyRigidBodyCollision(true);

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);
	MissileMesh->SetCollisionProfileName(FName("NoCollision"));

	MissileParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MissileParticles"));
	MissileParticles->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->bIsHomingProjectile = true;
	MovementComponent->HomingAccelerationMagnitude = 1000.0F;

	InitialLifeSpan = 4.0F;
}

// Called when the game starts or when spawned
void AMagicMissile::BeginPlay()
{
	Super::BeginPlay();
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMagicMissile::BeginSphereOverlap);
	SphereCollision->OnComponentHit.AddDynamic(this, &AMagicMissile::SphereHit);
}

void AMagicMissile::BeginSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AOrcCharacter* Orc = Cast<AOrcCharacter>(OtherActor);
	if (Orc)
	{
		// Spawn Explosion and Sound
		if (ExplosionParticle)
		{
			FTransform ParticleTransform;
			ParticleTransform.SetLocation(GetRootComponent()->GetComponentLocation());
			ParticleTransform.SetRotation(GetRootComponent()->GetComponentQuat());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, ParticleTransform);
		}
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetRootComponent()->GetComponentLocation());
		}
		// Deal damage
		Orc->KillOrc();
		Destroy();
	}
}

void AMagicMissile::SphereHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	AOrcCharacter* Orc = Cast<AOrcCharacter>(OtherActor);
	if (Orc)
	{
		// Deal damage
		Orc->KillOrc();
	}
	// Spawn Explosion and Sound
	if (ExplosionParticle)
	{
		FTransform ParticleTransform;
		ParticleTransform.SetLocation(GetRootComponent()->GetComponentLocation());
		ParticleTransform.SetRotation(GetRootComponent()->GetComponentQuat());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, ParticleTransform);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetRootComponent()->GetComponentLocation());
	}
	Destroy();
}
