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
    dungeonGenerator = new FDungeonGenerator();
    
}

void ADungeonMaker::MakeField()
{
    if (dungeonGenerator == nullptr)
    {
        dungeonGenerator = new FDungeonGenerator();
    }
    
    dungeonGenerator->Generate(MapHorizontal, MapVertical, FMath::RandRange(1, 100));

    for (int32 row = 0; row < MapVertical; ++row)
    {
        for (int32 column = 0; column < MapHorizontal; ++column)
        {
            AsyncTask(ENamedThreads::GameThread, [row, column, this]()
            {
                if(dungeonGenerator->GetTile(column, row) == EDungeonTile::Wall)
                    this->SpawnAreaBP(column, row);
            });
        }
    }
}

void ADungeonMaker::SpawnAreaBP_Implementation(int32 horizontal, int32 vertical)
{

}
