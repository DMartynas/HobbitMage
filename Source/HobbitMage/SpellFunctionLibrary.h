// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HOBBITMAGE_API USpellFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float DynamicTimeWarping(TArray<FVector2D> circle, TArray<FVector2D> fit);

	TArray<FVector2D> SlideArray(TArray<FVector2D> circle, TArray<FVector2D> fit);

	
};
