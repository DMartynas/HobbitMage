// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagePawn.h"
#include <ctime>
#include <chrono>
#include "CoreMinimal.h"

using namespace std::chrono;
/**
 * 
 */
static struct  HOBBITMAGE_API FSpellDetector
{
	

public:
	static bool DetectCircle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius, AMagePawn* Mage);
	static bool DetectTriangle(const TArray<FVector> &Positions, FVector &triangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage);
	static bool DetectLightning(const TArray<FVector> &Positions, FVector &triangleCenter, int &counter, bool &resetTime, system_clock::time_point timeStarted, AMagePawn* Mage);
	static int DetectSlashOrStab(const TArray<FVector> &Positions, AMagePawn* Mage, FVector &OutSlashVector1, FVector &OutSlashVector2);
	static void RecordMovement(const TArray<FVector> &Positions, AMagePawn* Mage, FVector &OutSlashVector1, FVector &OutSlashVector2);
	static bool DetectShallNotPass(const TArray<FVector> &Positions, FVector HeadLocation, float StaffHalfHeight);
	static void restart(int &counter, bool &resetTime, float &totalTime);
	static float minimum(float first, float second, float third);
	//static FString ReadFromFile();


public:
	FVector circle[];
};