// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LIKEEE_API UDungeonBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static void DestroyAndClearActors(UPARAM(ref) TArray<AActor*>& Actors);
	
};
