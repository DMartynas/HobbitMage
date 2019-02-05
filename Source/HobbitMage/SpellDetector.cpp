// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellDetector.h"
#include "Engine.h"

bool FSpellDetector::DetectCircle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius)
{
	FVector AverageLocation = FVector::ZeroVector;
	float AverageDistance = 0.0F;
	float IsCircle = 0.0F;

	if (Positions.Num() != 0)
	{

		//Attempt to see where the center of the circle is
		for (int i = 0; i < Positions.Num(); i++)
		{
			AverageLocation += Positions[i];
		}

		AverageLocation /= Positions.Num();

		// Attempt to get the point distances to the circle
		TArray<float> Distances;
		for (int i = 0; i < Positions.Num(); i++)
		{
			float Distance = FVector::Distance(AverageLocation, Positions[i]);
			Distances.Add(Distance);
			AverageDistance += Distance;
		}
		AverageDistance /= Positions.Num();

		CircleRadius = AverageDistance;

		// We have a minimum radius of a circle set
		if (AverageDistance >= 15.0F)
		{
			int Counter = 0;
			for (int i = 0; i < Distances.Num(); i++)
			{
				float LowBound = AverageDistance * (1 - RadiusVariation);
				float HighBound = AverageDistance * (1 + RadiusVariation);

				if (Distances[i] > LowBound && Distances[i] < HighBound)
				{
					Counter++;
				}
			}
			IsCircle = (float)Counter / (float)Distances.Num();
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Avg Loc: %s, Avg Dist: %f, Is circle: %f"), *AverageLocation.ToString(), AverageDistance, IsCircle);

	OutAverageLoc = AverageLocation;

	return IsCircle >= AcceptanceThreshold;
}

bool FSpellDetector::DetectTriangle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius)
{
	FVector AverageLocation = FVector::ZeroVector;
	float AverageDistance = 0.0F;
	float IsTriangle = 0.0F;
	bool pos1 = false;
	bool pos2 = false;
	bool pos3 = false;
	if (Positions.Num() != 0)
	{

		//Attempt to see where the center of the circle is
		for (int i = 0; i < Positions.Num(); i++)
		{
			if (i == 0) continue;
			/*UE_LOG(LogTemp, Warning, TEXT("position: %f x; %f y; %f z;"), Positions[i].X, Positions[i].Y, Positions[i].Z);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("position: %f x; %f y; %f z;"), Positions[i].X, Positions[i].Y, Positions[i].Z));*/

			//Check movement pattern
			if (Positions[i - 1].Y - Positions[i].Y > AcceptanceThreshold && Positions[i - 1].Z - Positions[i].Z > AcceptanceThreshold)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Y ir Z mazeja")));
				pos1 = true;
			}

			if (Positions[i].Y - Positions[i - 1].Y > AcceptanceThreshold && Positions[i - 1].Z - Positions[i].Z < AcceptanceThreshold)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Z nejuda")));
				pos2 = true;
			}
			if (Positions[i - 1].Y - Positions[i].Y > AcceptanceThreshold && Positions[i].Z - Positions[i - 1].Z > AcceptanceThreshold)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Y mazeja Z dideja")));
				pos3 = true;
			}

			AverageLocation += Positions[i];
		}
		
		OutAverageLoc = AverageLocation / Positions.Num();

	}
	return pos1 && pos2 && pos3;
}

	bool FSpellDetector::IsThereAnAngle(FVector Position1, FVector Position2)
	{
		float x = Position1.X - Position2.X;
		float y = Position1.Y - Position2.Y;
		float z = Position1.Z - Position2.Z;


		return false;
	}

	bool FSpellDetector::DetectShallNotPass(const TArray<FVector>& Positions, FVector HeadLocation, float StaffHalfHeight)
{
	float HeightThreshold = HeadLocation.Z + StaffHalfHeight;
	float StaffHeight = 0.0F;
	if (Positions.Num() > 0)
	{
		for (int i = 0; i < Positions.Num(); i++)
		{
			StaffHeight += Positions[i].Z;
		}

		StaffHeight /= Positions.Num();
	}

	return StaffHeight >= HeightThreshold;
}
