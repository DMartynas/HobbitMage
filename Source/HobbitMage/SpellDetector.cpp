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
	FVector firstPoint;
	FVector lastPoint;
	float AverageDistance = 0.0F;
	float IsCircle = 0.0F;
	bool rt = true;
	system_clock::time_point timeStarted;
	if (Positions.Num() > 15)
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
			if (i == 0) firstPoint = Positions[i];
			if (FVector::Distance(firstPoint, Positions[i]) < 5) lastPoint = Positions[i];
			float Distance = FVector::Distance(AverageLocation, Positions[i]);
			Distances.Add(Distance);
			AverageDistance += Distance;
		}
		AverageDistance /= Positions.Num();

		CircleRadius = AverageDistance;

		// We have a minimum radius of a circle set
		if (AverageDistance >= 13.0F)
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
			if (IsCircle >= AcceptanceThreshold)
				return true;
		}
	}


	
	if (FVector::Distance(firstPoint, lastPoint) < 4 && FVector::Distance(firstPoint, lastPoint) > 0 && (float)Positions.Num() > 30)
	{
		
		OutAverageLoc = AverageLocation + Mage->PlayerCamera->GetForwardVector() * 70.0F;
		system_clock::time_point timeStoped = system_clock::now();
		std::chrono::duration<double> dur = timeStoped - timeStarted;
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Time: %.9f"), dur.count()));
		Mage->debugMessage = FString::SanitizeFloat(FVector::Distance(firstPoint, lastPoint)) + " Acc: " + FString::SanitizeFloat(IsCircle);
		return IsCircle >= AcceptanceThreshold;
	}
	else return false;
}

/*bool FSpellDetector::DetectCircle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius, AMagePawn* Mage)
{

	return false;
}
*/
float FSpellDetector::minimum(float first, float second, float third)
{
	if (first <= second && first <= third)
		return first;
	if (second <= first && second <= third)
		return second;
	if (third <= first && third <= second)
		return third;
	return first;
}

FVector* StringToVector(FString Text)
{
	FVector* output = new FVector[20];
	
	return output;
}


bool FSpellDetector::DetectTriangle(const TArray<FVector> &Positions, FVector &OutTriangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage)
{
	FVector2D pos1 = FVector2D::ZeroVector;
	FVector2D pos2 = FVector2D::ZeroVector;
	FVector2D nnpos1 = FVector2D::ZeroVector;
	FVector2D nnpos2 = FVector2D::ZeroVector;
	FVector2D firstPoint = FVector2D::ZeroVector;
	FVector2D endPoint = FVector2D::ZeroVector;
	float minVectorLenght = 0;
	float totalTime = 0;
	FVector2D previous = previous.ZeroVector;
	FVector2D current = previous.ZeroVector;
	FVector2D nnprevious = previous.ZeroVector;
	FVector2D nncurrent = previous.ZeroVector;
	system_clock::time_point timeStoped;
	bool rt = true;
	//Mage->debugMessage = ReadFromFile();
	if (Positions.Num() > 17)
	{


		for (int i = 1; i < Positions.Num(); i++)
		{
			if (previous == previous.ZeroVector)
			{
				FVector temp = UKismetMathLibrary::ProjectPointOnToPlane(Positions[i], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector()) - UKismetMathLibrary::ProjectPointOnToPlane(Positions[i - 1], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector());
				previous = FVector2D(temp.X, temp.Z);
				
				if (previous.Size() <= minVectorLenght)
				{
					previous = FVector2D::ZeroVector;
				}
					if (counter == 0 && previous != previous.ZeroVector) { firstPoint = previous; }
					if (counter == 2 && previous != previous.ZeroVector) { endPoint = previous; }
					if (resetTime)
					{
						timeStarted = system_clock::now();
						resetTime = false;
					}
			}
			else if (previous != previous.ZeroVector)
			{
				
				FVector temp = UKismetMathLibrary::ProjectPointOnToPlane(Positions[i], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector()) - UKismetMathLibrary::ProjectPointOnToPlane(Positions[i - 1], Mage->PlayerCamera->GetComponentLocation(), Mage->PlayerCamera->GetForwardVector());
				current = FVector2D(temp.X, temp.Z);
				if (current.Size() <= minVectorLenght)
				{
					previous = previous.ZeroVector;
					current = current.ZeroVector;
				}
				else 
				{
					if (counter == 2 && current != current.ZeroVector) { endPoint = current; }
					
					
					nncurrent = current;
					nnprevious = previous;
					current.Normalize();
					previous.Normalize();
					float angle = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(current, previous)));
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Angle: %f"), angle));
					if(angle > 120.f) Mage->callDetectCircleFunc = false;
						else Mage->callDetectCircleFunc = true;
					
					UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), angle);
					if (angle > 90.f && angle < 150.f)
					{
						if (pos1 == FVector2D::ZeroVector)
						{
							pos1 = current;
							nnpos1 = nncurrent;
						}
						else if (pos2 == FVector2D::ZeroVector)
						{
							pos2 = current;
							nnpos2 = nncurrent;
							float distance = FVector2D::Distance(nnpos1, nnpos2);
							
							if (distance > 1.1f)
							{
								timeStoped = system_clock::now();
								std::chrono::duration<double> dur = timeStoped - timeStarted;
								if (dur.count() > 0.0)
								{
									counter++;
									
									totalTime += dur.count();
									
									/*if (counter > 2 /*&& totalTime < 0.000000008f)
									{
										restart(counter, resetTime, totalTime);
										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai: %f"), totalTime));
									}

									else if (totalTime > 0.1f)
									{
										restart(counter, resetTime, totalTime);
										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko perdaug: %f"), totalTime));
									}*/
									
									if (counter > 2 )
									{
										float distanceToFirstPoint = FVector2D::Distance(firstPoint, endPoint);
										float totalDistance = FVector2D::Distance(firstPoint, nnpos1) + FVector2D::Distance(nnpos1, endPoint) + FVector2D::Distance(endPoint, firstPoint);
										
										if (distanceToFirstPoint < 10.f && distanceToFirstPoint > 0.5f && totalDistance > 18.f && totalDistance < 40.f)
										{
											if ((float)Positions.Num() > 17)
											{
												restart(counter, resetTime, totalTime);

												OutTriangleCenter = Mage->PlayerCamera->GetComponentLocation() + Mage->PlayerCamera->GetForwardVector() * 100;
												//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), totalDistance));
												//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f"), distanceToFirstPoint, totalDistance));
												Mage->debugMessage = "DTFP: " + FString::SanitizeFloat(distanceToFirstPoint) + "TD: " + FString::SanitizeFloat(totalDistance);
												return true;
											}
										}
										else
										{
											Mage->debugMessage = "FAILED: DTFP: " + FString::SanitizeFloat(distanceToFirstPoint) + "TD: " + FString::SanitizeFloat(totalDistance);
											restart(counter, resetTime, totalTime);
											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Paskutinis")));
										}
										
									}
								}
								//else GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai")));
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
										
										if (distanceToFirstPoint > 5 && distanceToFirstPoint < 70.f && totalDistance > 18.5f && totalDistance < 30.f)
										{
											restart(counter, resetTime, totalTime);

											OuttriangleCenter = Mage->PlayerCamera->GetComponentLocation() + Mage->PlayerCamera->GetForwardVector() * 100.0F;
											
											GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f"), distanceToFirstPoint, totalDistance));
											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Veikia-------------")));
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
			
			//Mage->debugMessage = "X: " + FString::SanitizeFloat(abs(vector2.X - vector1.X)) + " Y: " + FString::SanitizeFloat(abs(vector2.Y - vector1.Y)) + " Z: " + FString::SanitizeFloat(abs(vector2.Z - vector1.Z));
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), *(Mage->debugMessage)));
			if (vector1 != FVector::ZeroVector && vector2 != FVector::ZeroVector)
			{
				if (abs(vector2.X - vector1.X) > 5 && abs(vector2.Z - vector1.Z) < 2 && (Mage->changeWeaponToHammer == false &&  Mage->changeWeaponToSword == false))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("------------------SLASH---------------------")));
					vector1 = FVector::ZeroVector;
					vector2 = FVector::ZeroVector;
					return 1;
				}
				else if (abs(vector2.Z - vector1.Z) > 6 && ( Mage->changeWeaponToHammer == false && Mage->changeWeaponToSword == false))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("------------------BASH---------------------")));
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

void FSpellDetector::RecordMovement(const TArray<FVector>& Positions, AMagePawn * Mage, FVector & OutSlashVector1, FVector & OutSlashVector2)
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
			if (OutSlashVector1 == FVector::ZeroVector)
				OutSlashVector1 = point1 - point2;
			else if (OutSlashVector2 == FVector::ZeroVector)
				OutSlashVector2 = point1 - point2;

			//Mage->debugMessage = "X: " + FString::SanitizeFloat(OutSlashVector2.X - OutSlashVector1.X) + " Y: " + FString::SanitizeFloat(OutSlashVector2.Y - OutSlashVector1.Y) + " Z: " + FString::SanitizeFloat(OutSlashVector2.Z - OutSlashVector1.Z);
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), *(Mage->debugMessage)));
			if (OutSlashVector1 != FVector::ZeroVector && OutSlashVector2 != FVector::ZeroVector)
			{
				if (OutSlashVector2.X - OutSlashVector1.X > 1 /*&& abs(vector2.Y - vector1.Y) < 10 && abs(vector2.Z - vector1.Z) < 2*/)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("------------------X>5---------------------")));
					OutSlashVector1 = FVector::ZeroVector;
					OutSlashVector2 = FVector::ZeroVector;
				}
				else if (OutSlashVector2.X - OutSlashVector1.X < -1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("------------------X<-5---------------------")));
					OutSlashVector1 = FVector::ZeroVector;
					OutSlashVector2 = FVector::ZeroVector;
				}
				else if (OutSlashVector2.Y - OutSlashVector1.Y > 1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("------------------DESINE---------------------")));
					OutSlashVector1 = FVector::ZeroVector;
					OutSlashVector2 = FVector::ZeroVector;
				}
				else if (OutSlashVector2.Y - OutSlashVector1.Y < -1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("------------------KAIRE---------------------")));
					OutSlashVector1 = FVector::ZeroVector;
					OutSlashVector2 = FVector::ZeroVector;
				}
				else if (OutSlashVector2.Z - OutSlashVector1.Z > 7)
				{

				}
				OutSlashVector1 = FVector::ZeroVector;
				OutSlashVector2 = FVector::ZeroVector;

			}

		}
	}
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

//
//bool FSpellDetector::DetectTriangle(const TArray<FVector> &Positions, FVector &OutTriangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage)
//{
//	FVector pos1 = FVector::ZeroVector;
//	FVector pos2 = FVector::ZeroVector;
//	FVector nnpos1 = FVector::ZeroVector;
//	FVector nnpos2 = FVector::ZeroVector;
//	FVector firstPoint = FVector::ZeroVector;
//	FVector endPoint = FVector::ZeroVector;
//	float minVectorLenght = 0.8f;
//	float totalTime = 0;
//	FVector previous = previous.ZeroVector;
//	FVector current = previous.ZeroVector;
//	FVector nnprevious = previous.ZeroVector;
//	FVector nncurrent = previous.ZeroVector;
//	system_clock::time_point timeStoped;
//	bool rt = true;
//	Mage->callDetectCircleFunc = true;
//	//Mage->debugMessage = ReadFromFile();
//	if (Positions.Num() != 0)
//	{
//
//
//		for (int i = 1; i < Positions.Num(); i++)
//		{
//			if (previous == previous.ZeroVector)
//			{
//				previous = Positions[i] - Positions[i - 1];
//				if (previous.Size() <= minVectorLenght)
//				{
//					previous = FVector::ZeroVector;
//				}
//					if (counter == 0 && previous != previous.ZeroVector) { firstPoint = previous; }
//					if (counter == 2 && previous != previous.ZeroVector) { endPoint = previous; }
//					nnprevious = previous;
//					previous.Normalize();
//					if (resetTime)
//					{
//						timeStarted = system_clock::now();
//						resetTime = false;
//					}
//			}
//			else if (previous != previous.ZeroVector)
//			{
//				current = Positions[i] - Positions[i - 1];
//				if (current.Size() <= minVectorLenght)
//				{
//					previous = previous.ZeroVector;
//					current = current.ZeroVector;
//				}
//				else
//				{
//					if (counter == 2 && current != current.ZeroVector) { endPoint = current; }
//
//					if (current.Size()!= 1)
//					{
//						nncurrent = current;
//						current.Normalize();
//					}
//					//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("X: %f | Y: %f"), abs(nncurrent.X - nnprevious.X), abs(nncurrent.Y - nnprevious.Y)));
//
//
//					float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(current, previous)));
//					
////					if(angle > 120.f) Mage->callDetectCircleFunc = false;
////						//else Mage->callDetectCircleFunc = true;
////					UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), angle);
////					if (angle > 90.f && angle < 150.f)
//					{
//						
//						if (pos1 == FVector::ZeroVector)
//						{
//							pos1 = current;
//							nnpos1 = nncurrent;
//						}
//						else if (pos2 == FVector::ZeroVector)
//						{
//							pos2 = current;
//							nnpos2 = nncurrent;
//							float distance = FVector::Distance(nnpos1, nnpos2);
//							UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
//							if (distance > 1.f)
//							{
//								timeStoped = system_clock::now();
//								std::chrono::duration<double> dur = timeStoped - timeStarted;
//								if (dur.count() > 0.0)
//								{
//									counter++;
//
//									totalTime += dur.count();
//
//									/*if (counter > 2 /*&& totalTime < 0.000000008f)
//									{
//										restart(counter, resetTime, totalTime);
//										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai: %f"), totalTime));
//									}
//
//									else if (totalTime > 0.1f)
//									{
//										restart(counter, resetTime, totalTime);
//										GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko perdaug: %f"), totalTime));
//									}*/
//
//									if (counter > 2)
//									{
//										float distanceToFirstPoint = FVector::Distance(firstPoint, endPoint);
//										float totalDistance = FVector::Distance(firstPoint, nnpos1) + FVector::Distance(nnpos1, endPoint) + FVector::Distance(endPoint, firstPoint);
//
//										Mage->debugMessage = "DTFP: " + FString::SanitizeFloat(distanceToFirstPoint) + "TD: " + FString::SanitizeFloat(totalDistance) + "A: " + FString::SanitizeFloat(angle);
//										if (distanceToFirstPoint < 10.f && distanceToFirstPoint >= 0.f && totalDistance > 18.f && totalDistance < 60.f)
//										{
//											if ((float)Positions.Num() > 17)
//											{
//												restart(counter, resetTime, totalTime);
//												GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f, angle: %f"), distanceToFirstPoint, totalDistance, angle));
//												OutTriangleCenter = Mage->PlayerCamera->GetComponentLocation() + Mage->PlayerCamera->GetForwardVector() * 100;
//												//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), totalDistance));
//												UE_LOG(LogTemp, Warning, TEXT("TRIANGLE"));
//												//Mage->debugMessage = "DTFP: " + FString::SanitizeFloat(distanceToFirstPoint) + "TD: " + FString::SanitizeFloat(totalDistance);
//												return true;
//											}
//										}
//										else
//										{
//											GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("distanceToFirstPoint: %f, totalDistance: %f, angle: %f"), distanceToFirstPoint, totalDistance, angle));
//											restart(counter, resetTime, totalTime);
//											//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Paskutinis")));
//										}
//
//									}
//								}
//								else GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Laiko permazai")));
//							}
//						}
//					}
//					
//				}
//			}
//		}
//
//
//
//	}
//	//Mage->callDetectCircleFunc = true;
//	return false;
//}
//
