// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaker.h"

// Sets default values
ADungeonMaker::ADungeonMaker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonMaker::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonMaker::MakeField()
{
    for (int32 row = 0; row < MapVertical; ++row)
    {
        for (int32 column = 0; column < MapHorizontal; ++column)
        {
            AsyncTask(ENamedThreads::GameThread, [row, column, this]()
            {
                if(Map[row][column] == 0)
                    this->SpawnAreaBP(row, column);
            });
        }
    }
}

void ADungeonMaker::CreateObject(int32 vertical, int32 horizontal)
{
    FVector Location(vertical * 100, horizontal * 100, 0.0f);
    FRotator Rotation = FRotator::ZeroRotator;
}

void ADungeonMaker::SpawnAreaBP_Implementation(int32 vertical, int32 horizontal)
{

}
