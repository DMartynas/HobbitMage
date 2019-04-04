// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellDetector.h"
#include "MagePawn.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include <vector>
#include <iostream>   
#include <string>  
#include "Engine.h"


bool FSpellDetector::DetectCircle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius, AMagePawn* Mage)
{
	
	FVector AverageLocation = FVector::ZeroVector;
	float AverageDistance = 0.0F;
	float IsCircle = 0.0F;
	bool rt = true;
	system_clock::time_point timeStarted;
	if (Positions.Num() != 0)
	{

		//Attempt to see where the center of the circle is
		for (int i = 0; i < Positions.Num(); i++)
		{
			AverageLocation += Positions[i];
			
		}

		AverageLocation /= Positions.Num();
		if (rt)
		{
			timeStarted = system_clock::now();
			rt = false;
		}
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
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Counter: %f"), IsCircle));
			
		}
	}



	OutAverageLoc = AverageLocation + Mage->PlayerCamera->GetForwardVector() * 50.0F;;
	system_clock::time_point timeStoped = system_clock::now();
	std::chrono::duration<double> dur = timeStoped - timeStarted;
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Time: %.9f"), dur.count()));
	return IsCircle >= AcceptanceThreshold;
}

bool FSpellDetector::DetectTriangle(const TArray<FVector> &Positions, FVector &OutTriangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage)
{
	FVector pos1 = FVector::ZeroVector;
	FVector pos2 = FVector::ZeroVector;
	FVector nnpos1 = FVector::ZeroVector;
	FVector nnpos2 = FVector::ZeroVector;
	FVector firstPoint = FVector::ZeroVector;
	FVector endPoint = FVector::ZeroVector;
	float minVectorLenght = 0.9f;
	float totalTime = 0;
	FVector previous = previous.ZeroVector;
	FVector current = previous.ZeroVector;
	FVector nnprevious = previous.ZeroVector;
	FVector nncurrent = previous.ZeroVector;
	system_clock::time_point timeStoped;
	bool rt = true;
	if (Positions.Num() != 0)
	{


		for (int i = 1; i < Positions.Num(); i++)
		{
			if (previous == previous.ZeroVector)
			{
				previous = Positions[i] - Positions[i - 1];
				if (previous.Size() <= minVectorLenght)
				{
					previous = FVector::ZeroVector;
				}
					if (counter == 0 && previous != previous.ZeroVector) { firstPoint = previous; }
					if (counter == 2 && previous != previous.ZeroVector) { endPoint = previous; }
					nnprevious = previous;
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
					nncurrent = current;
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("X: %f | Y: %f"), abs(nncurrent.X - nnprevious.X), abs(nncurrent.Y - nnprevious.Y)));
					current.Normalize();
				
					float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(current, previous)));
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Angle: %f"), angle));
					if (angle > 10.f && angle < 100.f)
					{
						if (pos1 == FVector::ZeroVector)
						{
							pos1 = current;
							nnpos1 = nncurrent;
						}
						else if (pos2 == FVector::ZeroVector)
						{
							pos2 = current;
							nnpos2 = nncurrent;
							float distance = FVector::Distance(nnpos1, nnpos2);
							if (distance > 1.1f)
							{
								timeStoped = system_clock::now();
								std::chrono::duration<double> dur = timeStoped - timeStarted;
								if (dur.count() > 0.0)
								{
									counter++;
									
									totalTime += dur.count();
									
									if (counter > 2 && totalTime < 0.000000008f)
									{
										restart(counter, resetTime, totalTime);
										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai: %f"), totalTime));
									}

									else if (totalTime > 0.00001f)
									{
										restart(counter, resetTime, totalTime);
										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko perdaug: %f"), totalTime));
									}
									
									else if (counter > 2 )
									{
										float distanceToFirstPoint = FVector::Distance(firstPoint, endPoint);
										float totalDistance = FVector::Distance(firstPoint, nnpos1) + FVector::Distance(nnpos1, endPoint) + FVector::Distance(endPoint, firstPoint);
										
										if (distanceToFirstPoint < 7 && distanceToFirstPoint > 1.5f && totalDistance > 17.9f && totalDistance < 25.f)
										{
											restart(counter, resetTime, totalTime);

											OutTriangleCenter = Mage->PlayerCamera->GetComponentLocation() + Mage->PlayerCamera->GetForwardVector() * 100.0F;
											
											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f"), distanceToFirstPoint, totalDistance));
											return true;
										}
										else
										{
											restart(counter, resetTime, totalTime);
											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Paskutinis")));
										}
										
									}
								}
								else GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai")));
							}
						}
					}
				}
			}
		}
		
		

	}
	return false;
}

bool FSpellDetector::DetectLightning(const TArray<FVector> &Positions, FVector &OuttriangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage)
{
	FVector pos1 = FVector::ZeroVector;
	FVector pos2 = FVector::ZeroVector;
	FVector nnpos1 = FVector::ZeroVector;
	FVector nnpos2 = FVector::ZeroVector;
	FVector firstPoint = FVector::ZeroVector;
	FVector endPoint = FVector::ZeroVector;
	float minVectorLenght = 0.8f;
	float totalTime = 0;
	FVector previous = previous.ZeroVector;
	FVector current = previous.ZeroVector;
	FVector nnprevious = previous.ZeroVector;
	FVector nncurrent = previous.ZeroVector;
	system_clock::time_point timeStoped;
	bool rt = true;
	if (Positions.Num() != 0)
	{


		for (int i = 1; i < Positions.Num(); i++)
		{
			if (previous == previous.ZeroVector)
			{
				previous = Positions[i] - Positions[i - 1];
				if (previous.Size() <= minVectorLenght)
				{
					previous = FVector::ZeroVector;
				}
					if (counter == 0 && previous != previous.ZeroVector) { firstPoint = previous; }
					if (counter == 2 && previous != previous.ZeroVector) { endPoint = previous; }
					nnprevious = previous;
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
					nncurrent = current;
					current.Normalize();
				
					float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(current, previous)));
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Angle: %f"), angle));
					if (angle > 10.f && angle < 100.f)
					{
						if (pos1 == FVector::ZeroVector)
						{
							pos1 = current;
							nnpos1 = nncurrent;
						}
						else if (pos2 == FVector::ZeroVector)
						{
							pos2 = current;
							nnpos2 = nncurrent;
							float distance = FVector::Distance(nnpos1, nnpos2);
							if (distance > 1.1f)
							{
								timeStoped = system_clock::now();
								std::chrono::duration<double> dur = timeStoped - timeStarted;
								if (dur.count() > 0.0)
								{
									counter++;
									
									totalTime += dur.count();
									
									if (counter > 2 && totalTime < 0.000000008f)
										restart(counter, resetTime, totalTime);

									else if (totalTime > 0.00001f)
										restart(counter, resetTime, totalTime);
									
									else if (counter > 2 )
									{
										float distanceToFirstPoint = pow(firstPoint.Z - endPoint.Z, 2);
										float totalDistance = FVector::Distance(firstPoint, nnpos1) + FVector::Distance(nnpos1, endPoint) + FVector::Distance(endPoint, firstPoint);
										
										if (distanceToFirstPoint > 40 && distanceToFirstPoint < 70.f && totalDistance > 26.f && totalDistance < 30.f)
										{
											restart(counter, resetTime, totalTime);

											OuttriangleCenter = Mage->PlayerCamera->GetComponentLocation() + Mage->PlayerCamera->GetForwardVector() * 100.0F;
											
											GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f"), distanceToFirstPoint, totalDistance));
											GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Veikia-------------")));
											return true;
										}
										else restart(counter, resetTime, totalTime);
										
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

int FSpellDetector::DetectSlashOrStab(const TArray<FVector>& Positions, AMagePawn * Mage, FVector &vector1, FVector &vector2)
{
	FVector point1;
	FVector point2;
	Mage->PlayerCamera->GetForwardVector();
	
	for (int i = 1; i < Positions.Num(); i++)
	{
		if (i == Positions.Num() - 2)
		{
			point1 = UKismetMathLibrary::ProjectPointOnToPlane(Positions[i], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector());
			point2 = UKismetMathLibrary::ProjectPointOnToPlane(Positions[i + 1], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector());
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), point1.X, point1.Y, point1.Z));
			if (vector1 == FVector::ZeroVector)
				vector1 = point1 - point2;
			else if (vector2 == FVector::ZeroVector)
				vector2 = point1 - point2;
			
			Mage->debugMessage = "X: " + FString::SanitizeFloat(abs(vector2.X - vector1.X)) + " Y: " + FString::SanitizeFloat(abs(vector2.Y - vector1.Y)) + " Z: " + FString::SanitizeFloat(abs(vector2.Z - vector1.Z));
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), *(Mage->debugMessage)));
			if (vector1 != FVector::ZeroVector && vector2 != FVector::ZeroVector)
			{
				if (abs(vector2.X - vector1.X) > 5 /*&& abs(vector2.Y - vector1.Y) < 10 && abs(vector2.Z - vector1.Z) < 2*/)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("------------------SLASH---------------------")));
					vector1 = FVector::ZeroVector;
					vector2 = FVector::ZeroVector;
					return 1;
				}
				else if (abs(vector2.Z - vector1.Z) > 7)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("------------------BASH---------------------")));
					vector1 = FVector::ZeroVector;
					vector2 = FVector::ZeroVector;
					return 2;
				}
				vector1 = FVector::ZeroVector;
				vector2 = FVector::ZeroVector;
				
			}

		}  
	}
	return 0;
}

void FSpellDetector::DetectStab(const TArray<FVector>& Positions, AMagePawn * Mage, FVector & OutSlashVector1, FVector & OutSlashVector2)
{
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

void FSpellDetector::restart(int & counter, bool & resetTime, float & totalTime)
	{
		counter = 0;
		resetTime = true;
		totalTime = 0;
	}
