// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	static bool DetectCircle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius);
	static bool DetectTriangle(const TArray<FVector> &Positions, float AcceptanceThreshold, float RadiusVariation, FVector &OutAverageLoc, float &CircleRadius, int &counter, bool &resetTime, system_clock::time_point timeStarted);
	static bool GetTriangle(const TArray<FVector> &Positions);
	static bool DetectShallNotPass(const TArray<FVector> &Positions, FVector HeadLocation, float StaffHalfHeight);
};