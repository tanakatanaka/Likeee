// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// ========================================
// タイル
// ========================================

enum class EDungeonTile : uint8
{
	Wall,
	Floor
};


// ========================================
// 部屋
// ========================================

struct FDungeonRoom
{
	int32 X = 0;
	int32 Y = 0;
	int32 Width = 0;
	int32 Height = 0;

	int32 CenterX() const
	{
		return X + Width / 2;
	}

	int32 CenterY() const
	{
		return Y + Height / 2;
	}
};


// ========================================
// 区画
// ========================================

struct FDungeonBlock
{
	int32 X = 0;
	int32 Y = 0;
	int32 Width = 0;
	int32 Height = 0;

	FDungeonRoom Room;
};


// ========================================
// 接続情報
// ========================================

struct FDungeonConnection
{
	int32 A = 0;
	int32 B = 0;
};


// ========================================
// ダンジョン生成クラス
// ========================================

class LIKEEE_API FDungeonGenerator
{
public:

	FDungeonGenerator();
	~FDungeonGenerator();

	// ダンジョン生成
	void Generate(
		int32 InWidth,
		int32 InHeight,
		int32 Seed
	);

	// タイル取得
	EDungeonTile GetTile(
		int32 X,
		int32 Y
	) const;

	int32 GetWidth() const
	{
		return Width;
	}

	int32 GetHeight() const
	{
		return Height;
	}

	const TArray<FDungeonRoom>& GetRooms() const
	{
		return Rooms;
	}

private:

	// マップサイズ
	int32 Width = 0;
	int32 Height = 0;

	// 2次元マップ
	// Index = Y * Width + X
	TArray<EDungeonTile> Tiles;

	// 区画
	TArray<FDungeonBlock> Blocks;

	// 部屋
	TArray<FDungeonRoom> Rooms;

	// 接続
	TArray<FDungeonConnection> Connections;

	// UE5の乱数
	FRandomStream Random;


	// ========================================
	// 生成処理
	// ========================================

	void InitializeMap();

	void CreateBlocks();

	void CreateRooms();

	void DecideConnections();

	void CreateCorridors();

	bool CheckConnectivity() const;


	// ========================================
	// 通路
	// ========================================

	void DigHorizontal(
		int32 X1,
		int32 X2,
		int32 Y
	);

	void DigVertical(
		int32 Y1,
		int32 Y2,
		int32 X
	);


	// ========================================
	// Tile操作
	// ========================================

	void SetTile(
		int32 X,
		int32 Y,
		EDungeonTile Tile
	);

	bool IsInside(
		int32 X,
		int32 Y
	) const;
};