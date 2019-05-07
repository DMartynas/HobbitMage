// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellFunctionLibrary.h"
#include "SpellDetector.h"
#include <algorithm>
#include "Engine.h"

float USpellFunctionLibrary::DynamicTimeWarping(TArray<FVector2D> circle, TArray<FVector2D> fit)
{
	int n = circle.Num();
	int m = fit.Num();
	double** DTW = new double*[n];
	for (int i = 0; i < n; i++)
	{
		DTW[i] = new double[m];
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			DTW[i][j] = 999999999;
	DTW[0][0] = 0;

	TArray<FVector2D> correctedArray = USpellFunctionLibrary::SlideArray(circle, fit); //fit;
	for (int i = 1; i < n; i++)
		for (int j = 1; j < m; j++)
		{
			double cost;
			if(FVector2D::Distance(circle[i], correctedArray[j]) > 1.5f)
				cost = FVector2D::Distance(circle[i], correctedArray[j]);
			else cost = 999999999;
			if(i == 0 && j > 0)
				DTW[i][j] = cost + DTW[i][j - 1];
			else if (i > 0 && j == 0)
				DTW[i][j] = cost + DTW[i - 1][j];
			DTW[i][j] = cost + FSpellDetector::minimum(DTW[i - 1][j], DTW[i][j - 1], DTW[i - 1][j - 1]);
		}

	FString SaveDirectory = FString("C:/Users/VR-Lab-06/Desktop/matrix");
	FString FileName = "matrix" + FString::SanitizeFloat(DTW[n - 1][m - 1]) + ".csv";
	FString TextToSave = "";
	bool AllowOverwriting = true;


	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting)
		{
			for (int i = 1; i < n; i++)
			{
				for (int j = 1; j < m; j++)
				{
					TextToSave.Append(FString::SanitizeFloat(DTW[i][j]).Append(";"));
				}
				TextToSave.Append("\n");
			}
			FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
	}
	/*for (int i = n-1; i > 0; i--)
	{
		for (int j = m-1; j > 0; j--)
		{

			result += FSpellDetector::minimum(DTW[i - 1][j - 1], DTW[i][j - 1], DTW[i - 1][j]);
		}
		
	}*/
	fit.Empty();
	float result = DTW[n - 1][m - 1];
	DTW = new double*[n];
	return result;
}

TArray<FVector2D> USpellFunctionLibrary::SlideArray(TArray<FVector2D> circle, TArray<FVector2D> fit)
{
	float bestFit = FVector2D::Distance(circle[0], fit[0]);
	int iterator = 0;
	for (int i = 0; i < fit.Num(); i++)
	{
		if (FVector2D::Distance(circle[0], fit[i]) < bestFit)
		{
			bestFit = FVector2D::Distance(circle[0], fit[i]);
			iterator = i;
		}
	}
	if (iterator == 0)
		return fit;
	TArray<FVector2D> output;
	output.Init(FVector2D::ZeroVector, fit.Num());
	for (int i = 0; i < fit.Num() - iterator; i++)
		output[i] = FVector2D(fit[iterator + i]);
	
	for (int i = iterator; i < fit.Num(); i++)
		output[i] = fit[i - iterator];
	return output;
}