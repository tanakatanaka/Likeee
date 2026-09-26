// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonBlueprintFunctionLibrary.h"

void UDungeonBlueprintFunctionLibrary::DestroyAndClearActors(TArray<AActor*>& Actors)
{
    for (AActor* Actor : Actors)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }

    Actors.Empty();
}