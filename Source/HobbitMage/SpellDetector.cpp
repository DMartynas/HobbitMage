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

bool FSpellDetector::DetectTriangle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius, int &counter, bool &resetTime, system_clock::time_point timeStarted)
{
	FVector AverageLocation = FVector::ZeroVector;
	float AverageDistance = 0.0F;
	float IsTriangle = 0.0F;
	FVector pos1 = FVector::ZeroVector;
	FVector pos2 = FVector::ZeroVector;
	FVector firstPoint = FVector::ZeroVector;
	FVector endPoint = FVector::ZeroVector;
	float minVectorLenght = 0.6f;
	float totalTime = 0;
	FVector previous = previous.ZeroVector;
	FVector current = previous.ZeroVector;
	system_clock::time_point timeStoped;
	int avgLocCounter = 0;
	bool rt = true;
	if (Positions.Num() != 0)
	{


		for (int i = 1; i < Positions.Num(); i++)
		{
			/*if (i == 0) continue;
			UE_LOG(LogTemp, Warning, TEXT("position: %f x; %f y; %f z;"), Positions[i].X, Positions[i].Y, Positions[i].Z);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("position: %f x; %f y; %f z;"), Positions[i].X, Positions[i].Y, Positions[i].Z));

			//Check movement pattern
			if (Positions[i - 1].Y - Positions[i].Y > AcceptanceThreshold && Positions[i - 1].Z - Positions[i].Z > AcceptanceThreshold)
			{
				if (pos2 == true || pos3 == true)
				{
					pos1 = false;
					continue;
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Y ir Z mazeja")));
				pos1 = true;
			}
			if (Positions[i - 1].Y - Positions[i].Y > AcceptanceThreshold && Positions[i - 1].Z - Positions[i].Z < AcceptanceThreshold)
			{
				if (pos2 == true || pos3 == true)
				{
					pos1 = false;
					continue;
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Y dideja Z nesikeicia")));
				if (pos1 == true) pos2 = true;
			}
			if (Positions[i - 1].Y - Positions[i].Y < -5 && Positions[i - 1].Z - Positions[i].Z < -10)
			{
				pos1 = false;
				pos2 = false;
				pos3 = false;
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Y: %f;	 Z: %f;"), Positions[i - 1].Y - Positions[i].Y, Positions[i - 1].Z - Positions[i].Z));
			}
			if (Positions[i - 1].Y - Positions[i].Y > AcceptanceThreshold && Positions[i].Z - Positions[i - 1].Z > AcceptanceThreshold)
			{
				if (pos1 == false || pos2 == false)
				{
					pos3 = false;
					continue;
				}
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Y mazeja Z dideja")));
				if (pos1 == true && pos2 == true) pos3 = true;
			}*/
			
			/*timeStoped = high_resolution_clock::now();
			std::chrono::duration<double, std::milli> dur = timeStoped - timeStarted;
			if (dur.count() > 10000000.f)
			{
				if(counter > 0)
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("RESTARTED: %f"), dur.count()));
				totalTime = 0;
				counter = 0;
				timeStarted = high_resolution_clock::now();
				rt = false;
			}*/
			if (previous == previous.ZeroVector)
			{
				previous = Positions[i] - Positions[i - 1];
				if (previous.Size() <= minVectorLenght)
				{
					previous = FVector::ZeroVector;
				}
				AverageLocation += Positions[i];
				avgLocCounter++;
					if (counter == 0 && previous != previous.ZeroVector) { firstPoint = previous; }
					if (counter == 2 && previous != previous.ZeroVector) { endPoint = previous; }
					previous.Normalize();
					if (rt) 
					{
						timeStarted = system_clock::now();
						rt = false;
					}
			}
			else if (previous != previous.ZeroVector)
			{
				current = Positions[i] - Positions[i - 1];
				if (current.Size() <= minVectorLenght)
				{
					previous = previous.ZeroVector;
					current = current.ZeroVector;
				}
				else 
				{
					if (counter == 2 && current != current.ZeroVector) { endPoint = current; }
					current.Normalize();
				
					float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(current, previous)));
					if (angle > 50.f && angle < 70.f)
					{
						if (pos1 == FVector::ZeroVector)
							pos1 = current;
						else if (pos2 == FVector::ZeroVector)
						{
							pos2 = current;
							float distance = FVector::Distance(pos1, pos2);//sqrt(pow(pos2.X - pos1.X, 2) + pow(pos2.Y - pos1.Y, 2) + pow(pos2.Z - pos1.Z, 2));
							if (distance > 1.1f)
							{
								timeStoped = system_clock::now();;
								std::chrono::duration<double> dur = timeStoped - timeStarted;
								if (dur.count() > 0.0)
								{
									counter++;
									
									totalTime += dur.count();
									//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f | Total time: %.10f | Counter: %d"), distance, totalTime, counter));
									
									
									if (counter > 2 && totalTime < 0.000000008f)
									{
										float distanceToFirstPoint = FVector::Distance(firstPoint, endPoint);
										//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f "), distanceToFirstPoint));
										//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("RESTARTED")));
										totalTime = 0;
										rt = true;
										counter = 0;
										AverageLocation = AverageLocation.ZeroVector;
									}
									else if (totalTime > 0.00001f)
									{
										float distanceToFirstPoint = FVector::Distance(firstPoint, endPoint);
										//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f "), distanceToFirstPoint));
										//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("RESTARTED")));
										totalTime = 0;
										rt = true;
										counter = 0;
										AverageLocation = AverageLocation.ZeroVector;
									}
									else if (counter > 2 )
									{
										float distanceToFirstPoint = FVector::Distance(firstPoint, endPoint);
										//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f "), distanceToFirstPoint));
										if (distanceToFirstPoint < 9 && distanceToFirstPoint > 1.5f)
										{
											counter = 0;
											totalTime = 0;
											rt = true;
											AverageLocation.X += 90;
											OutAverageLoc = AverageLocation / avgLocCounter;
											return true;
										}
										else
										{
											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("RESTARTED")));
											totalTime = 0;
											rt = true;
											counter = 0;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		

	}
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
