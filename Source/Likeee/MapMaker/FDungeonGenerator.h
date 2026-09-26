// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// ============================================================
// タイルの種類
// ============================================================

enum class EDungeonTile : uint8
{
	Wall,
	Floor
};


// ============================================================
// 部屋
// ============================================================

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


// ============================================================
// 区画
// ============================================================

struct FDungeonBlock
{
	int32 X = 0;
	int32 Y = 0;

	int32 Width = 0;
	int32 Height = 0;

	FDungeonRoom Room;
};


// ============================================================
// 部屋同士の接続
// ============================================================

struct FDungeonConnection
{
	int32 A = 0;
	int32 B = 0;
};


// ============================================================
// ダンジョン生成クラス
// ============================================================

class LIKEEE_API FDungeonGenerator
{
public:

	FDungeonGenerator();
	~FDungeonGenerator();

	// ダンジョン生成
	void Generate(int32 InWidth, int32 InHeight, int32 Seed);

	// 指定位置のタイルを取得
	EDungeonTile GetTile(int32 X, int32 Y) const;

	// マップサイズ
	int32 GetWidth() const
	{
		return Width;
	}

	int32 GetHeight() const
	{
		return Height;
	}

	// 部屋一覧
	const TArray<FDungeonRoom>& GetRooms() const
	{
		return Rooms;
	}


private:

	// ========================================================
	// マップデータ
	// ========================================================

	int32 Width = 0;
	int32 Height = 0;

	// 2次元マップを1次元配列で保持
	//
	// Index = Y * Width + X
	TArray<EDungeonTile> Tiles;


	// ========================================================
	// ダンジョン構造
	// ========================================================

	TArray<FDungeonBlock> Blocks;
	TArray<FDungeonRoom> Rooms;
	TArray<FDungeonConnection> Connections;


	// ========================================================
	// 乱数
	// ========================================================

	FRandomStream Random;


	// ========================================================
	// 生成処理
	// ========================================================

	void InitializeMap();

	void CreateBlocks();
	void CreateRooms();

	void DecideConnections();
	void CreateCorridors();

	bool CheckConnectivity() const;


	// ========================================================
	// 通路生成
	// ========================================================

	void DigHorizontal(int32 X1, int32 X2, int32 Y);
	void DigVertical(int32 Y1, int32 Y2, int32 X);


	// ========================================================
	// タイル操作
	// ========================================================

	void SetTile(int32 X, int32 Y, EDungeonTile Tile);

	bool IsInside(int32 X, int32 Y) const;
};