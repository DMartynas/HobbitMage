// Fill out your copyright notice in the Description page of Project Settings.

#include "FireSpellCast.h"
#include "MagePawn.h"
#include "Engine/World.h"
#include "MagicMissile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EngineUtils.h"
#include "OrcCharacter.h"
#include "Kismet/GameplayStatics.h"

AFireSpellCast::AFireSpellCast(const FObjectInitializer &ObjInitializer) : Super(ObjInitializer)
{
	MagicMissileSpeedMultiplier = 40.0F;
}

void AFireSpellCast::CastSpell(AMagePawn* Mage, const FHitResult &HitResult)
{
	if (Mage)
	{
		FVector StaffVelocity = Mage->GetStaffVelocity();
		if (MissileClass != NULL)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FVector StaffVelDir = StaffVelocity;
				StaffVelDir.Normalize();
				FVector MagicMissileDirection = StaffVelDir;
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(GetActorLocation());
				SpawnTransform.SetRotation(StaffVelDir.ToOrientationQuat());
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = Mage;
				SpawnParams.Instigator = Mage;
				AMagicMissile* Missile = World->SpawnActor<AMagicMissile>(MissileClass, SpawnTransform, SpawnParams);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), CastSound, GetActorLocation());
				if (Missile)
				{
					float MinProduct = 9999.0F;
					AOrcCharacter* BestFitOrc = nullptr;
					for (TActorIterator<AOrcCharacter> Itr(World); Itr; ++Itr)
					{
						AOrcCharacter* Orc = *Itr;
						FVector DirToOrc = Orc->GetActorLocation() - GetActorLocation();
						DirToOrc.Normalize();
						float DotProduct = FVector::DotProduct(DirToOrc, StaffVelDir);
						if (DotProduct > 0.707F && DotProduct < MinProduct)
						{
							MinProduct = DotProduct;
							BestFitOrc = Orc;
							MagicMissileDirection = StaffVelDir + DirToOrc;
						}
					}
					MagicMissileDirection.Normalize();
					Missile->MovementComponent->Velocity = MagicMissileDirection * StaffVelocity.Size() * MagicMissileSpeedMultiplier;
					if (BestFitOrc)
					{
						Missile->MovementComponent->HomingTargetComponent = BestFitOrc->GetRootComponent();
					}
				}
			}
		}
	}
	Destroy();
}