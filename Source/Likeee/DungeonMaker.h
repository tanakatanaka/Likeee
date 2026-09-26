// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapMaker/FDungeonGenerator.h"
#include "DungeonMaker.generated.h"

UCLASS()
class LIKEEE_API ADungeonMaker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonMaker();

	//call BP Func
	UFUNCTION(BlueprintNativeEvent, Category = "MyCategory")
	void SpawnAreaBP(int32 horizontal, int32 vertical);
	virtual void SpawnAreaBP_Implementation(int32 horizontal, int32 vertical);

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MakeField();

protected:
	static constexpr int32 MapVertical = 30;
	static constexpr int32 MapHorizontal = 54;


private:
	FDungeonGenerator* dungeonGenerator;



};
