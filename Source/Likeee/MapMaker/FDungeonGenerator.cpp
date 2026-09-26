// Fill out your copyright notice in the Description page of Project Settings.

#include "MapMaker/FDungeonGenerator.h"

#include "Containers/Queue.h"


// ============================================================
// Constructor
// ============================================================

FDungeonGenerator::FDungeonGenerator()
{
}


// ============================================================
// Destructor
// ============================================================

FDungeonGenerator::~FDungeonGenerator()
{
}


// ============================================================
// Generate
// ============================================================

void FDungeonGenerator::Generate(int32 InWidth, int32 InHeight, int32 Seed)
{
	Width = InWidth;
	Height = InHeight;

	Random.Initialize(Seed);

	Blocks.Empty();
	Rooms.Empty();
	Connections.Empty();

	// ① マップをすべて壁にする
	InitializeMap();

	// ② 区画を作る
	CreateBlocks();

	// ③ 区画の中に部屋を作る
	CreateRooms();

	// ④ 部屋同士の接続関係を決める
	DecideConnections();

	// ⑤ 通路を掘る
	CreateCorridors();

	// ⑥ すべての床が接続されているか確認
	if (!CheckConnectivity())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dungeon is not connected!"));
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Dungeon Generated: %d x %d / Rooms=%d / Seed=%d"),
		Width,
		Height,
		Rooms.Num(),
		Seed
	);
}


// ============================================================
// マップ初期化
// ============================================================

void FDungeonGenerator::InitializeMap()
{
	Tiles.Init(EDungeonTile::Wall, Width * Height);
}


// ============================================================
// 区画生成
// ============================================================

void FDungeonGenerator::CreateBlocks()
{
	constexpr int32 TargetBlockCount = 8;

	constexpr int32 MinBlockWidth = 10;
	constexpr int32 MinBlockHeight = 8;


	// --------------------------------------------------------
	// 最初はマップ全体を1つの区画として扱う
	// --------------------------------------------------------

	FDungeonBlock Root;

	Root.X = 1;
	Root.Y = 1;

	Root.Width = Width - 2;
	Root.Height = Height - 2;

	Blocks.Add(Root);


	// --------------------------------------------------------
	// 区画を分割していく
	// --------------------------------------------------------

	int32 Attempts = 0;

	while (Blocks.Num() < TargetBlockCount && Attempts < 200)
	{
		++Attempts;

		// 分割する区画をランダムに選ぶ
		const int32 Index = Random.RandRange(0, Blocks.Num() - 1);

		const FDungeonBlock Block = Blocks[Index];


		// ----------------------------------------------------
		// 縦・横に分割可能か確認
		// ----------------------------------------------------

		const bool bCanVertical = Block.Width >= MinBlockWidth * 2;
		const bool bCanHorizontal = Block.Height >= MinBlockHeight * 2;

		if (!bCanVertical && !bCanHorizontal)
		{
			continue;
		}


		// ----------------------------------------------------
		// 分割方向を決定
		// ----------------------------------------------------

		bool bVertical = false;

		if (bCanVertical && bCanHorizontal)
		{
			// 横長なら縦方向に分割
			if (Block.Width > Block.Height * 1.5f)
			{
				bVertical = true;
			}

			// 縦長なら横方向に分割
			else if (Block.Height > Block.Width * 1.5f)
			{
				bVertical = false;
			}

			// 正方形に近いならランダム
			else
			{
				bVertical = Random.RandRange(0, 1) == 0;
			}
		}
		else
		{
			bVertical = bCanVertical;
		}


		// 元の区画を削除
		Blocks.RemoveAt(Index);


		// ====================================================
		// 縦方向に分割
		// ====================================================

		if (bVertical)
		{
			const int32 Split = Random.RandRange(
				MinBlockWidth,
				Block.Width - MinBlockWidth
			);


			FDungeonBlock A;

			A.X = Block.X;
			A.Y = Block.Y;

			A.Width = Split;
			A.Height = Block.Height;


			FDungeonBlock B;

			B.X = Block.X + Split;
			B.Y = Block.Y;

			B.Width = Block.Width - Split;
			B.Height = Block.Height;


			Blocks.Add(A);
			Blocks.Add(B);
		}


		// ====================================================
		// 横方向に分割
		// ====================================================

		else
		{
			const int32 Split = Random.RandRange(
				MinBlockHeight,
				Block.Height - MinBlockHeight
			);


			FDungeonBlock A;

			A.X = Block.X;
			A.Y = Block.Y;

			A.Width = Block.Width;
			A.Height = Split;


			FDungeonBlock B;

			B.X = Block.X;
			B.Y = Block.Y + Split;

			B.Width = Block.Width;
			B.Height = Block.Height - Split;


			Blocks.Add(A);
			Blocks.Add(B);
		}
	}
}


// ============================================================
// 部屋生成
// ============================================================

void FDungeonGenerator::CreateRooms()
{
	constexpr int32 MinRoomWidth = 4;
	constexpr int32 MinRoomHeight = 4;

	constexpr int32 Margin = 1;


	for (FDungeonBlock& Block : Blocks)
	{
		// ----------------------------------------------------
		// この区画に作れる最大の部屋サイズ
		// ----------------------------------------------------

		const int32 MaxRoomWidth = Block.Width - Margin * 2;
		const int32 MaxRoomHeight = Block.Height - Margin * 2;


		if (MaxRoomWidth < MinRoomWidth || MaxRoomHeight < MinRoomHeight)
		{
			continue;
		}


		// ----------------------------------------------------
		// 部屋サイズをランダム決定
		// ----------------------------------------------------

		const int32 RoomWidth = Random.RandRange(MinRoomWidth, MaxRoomWidth);
		const int32 RoomHeight = Random.RandRange(MinRoomHeight, MaxRoomHeight);


		// ----------------------------------------------------
		// 部屋を置ける範囲
		// ----------------------------------------------------

		const int32 MinX = Block.X + Margin;
		const int32 MaxX = Block.X + Block.Width - RoomWidth - Margin;

		const int32 MinY = Block.Y + Margin;
		const int32 MaxY = Block.Y + Block.Height - RoomHeight - Margin;


		// ----------------------------------------------------
		// 部屋位置をランダム決定
		// ----------------------------------------------------

		const int32 RoomX = Random.RandRange(MinX, MaxX);
		const int32 RoomY = Random.RandRange(MinY, MaxY);


		// ----------------------------------------------------
		// Room作成
		// ----------------------------------------------------

		FDungeonRoom Room;

		Room.X = RoomX;
		Room.Y = RoomY;

		Room.Width = RoomWidth;
		Room.Height = RoomHeight;


		// 区画に部屋を登録
		Block.Room = Room;

		// 部屋一覧にも登録
		Rooms.Add(Room);


		// ----------------------------------------------------
		// 部屋部分をFloorに変更
		// ----------------------------------------------------

		for (int32 Y = Room.Y; Y < Room.Y + Room.Height; ++Y)
		{
			for (int32 X = Room.X; X < Room.X + Room.Width; ++X)
			{
				SetTile(X, Y, EDungeonTile::Floor);
			}
		}
	}
}


// ============================================================
// 部屋同士の接続関係を決定
// ============================================================

void FDungeonGenerator::DecideConnections()
{
	Connections.Empty();


	if (Blocks.Num() <= 1)
	{
		return;
	}


	// 現段階では単純に
	//
	// 0 ── 1 ── 2 ── 3 ── 4
	//
	// のように全部つなぐ。
	//
	// 後でシレン風の接続グラフに変更可能。

	for (int32 i = 0; i < Blocks.Num() - 1; ++i)
	{
		FDungeonConnection Connection;

		Connection.A = i;
		Connection.B = i + 1;

		Connections.Add(Connection);
	}
}


// ============================================================
// 通路生成
// ============================================================

void FDungeonGenerator::CreateCorridors()
{
	for (const FDungeonConnection& Connection : Connections)
	{
		const FDungeonRoom& RoomA = Blocks[Connection.A].Room;
		const FDungeonRoom& RoomB = Blocks[Connection.B].Room;


		const int32 X1 = RoomA.CenterX();
		const int32 Y1 = RoomA.CenterY();

		const int32 X2 = RoomB.CenterX();
		const int32 Y2 = RoomB.CenterY();


		// ----------------------------------------------------
		// L字型の通路を作る
		// ----------------------------------------------------

		if (Random.RandRange(0, 1) == 0)
		{
			// 横 → 縦
			DigHorizontal(X1, X2, Y1);
			DigVertical(Y1, Y2, X2);
		}
		else
		{
			// 縦 → 横
			DigVertical(Y1, Y2, X1);
			DigHorizontal(X1, X2, Y2);
		}
	}
}


// ============================================================
// 横方向に通路を掘る
// ============================================================

void FDungeonGenerator::DigHorizontal(int32 X1, int32 X2, int32 Y)
{
	if (X1 > X2)
	{
		Swap(X1, X2);
	}


	for (int32 X = X1; X <= X2; ++X)
	{
		SetTile(X, Y, EDungeonTile::Floor);
	}
}


// ============================================================
// 縦方向に通路を掘る
// ============================================================

void FDungeonGenerator::DigVertical(int32 Y1, int32 Y2, int32 X)
{
	if (Y1 > Y2)
	{
		Swap(Y1, Y2);
	}


	for (int32 Y = Y1; Y <= Y2; ++Y)
	{
		SetTile(X, Y, EDungeonTile::Floor);
	}
}


// ============================================================
// BFSで全床がつながっているか確認
// ============================================================

bool FDungeonGenerator::CheckConnectivity() const
{
	int32 StartX = -1;
	int32 StartY = -1;

	int32 TotalFloor = 0;


	// --------------------------------------------------------
	// 最初の床を探す
	// --------------------------------------------------------

	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			if (GetTile(X, Y) != EDungeonTile::Floor)
			{
				continue;
			}

			++TotalFloor;


			if (StartX == -1)
			{
				StartX = X;
				StartY = Y;
			}
		}
	}


	// 床が1つもない
	if (StartX == -1)
	{
		return false;
	}


	// --------------------------------------------------------
	// 訪問済み配列
	// --------------------------------------------------------

	TArray<bool> Visited;

	Visited.Init(false, Width * Height);


	// --------------------------------------------------------
	// BFS開始
	// --------------------------------------------------------

	TQueue<FIntPoint> Queue;

	Queue.Enqueue(FIntPoint(StartX, StartY));

	Visited[StartY * Width + StartX] = true;


	static constexpr int32 DX[4] =
	{
		1,
		-1,
		0,
		0
	};

	static constexpr int32 DY[4] =
	{
		0,
		0,
		1,
		-1
	};


	int32 VisitedFloor = 0;


	// --------------------------------------------------------
	// BFS
	// --------------------------------------------------------

	while (!Queue.IsEmpty())
	{
		FIntPoint Current;

		Queue.Dequeue(Current);

		++VisitedFloor;


		for (int32 i = 0; i < 4; ++i)
		{
			const int32 NX = Current.X + DX[i];
			const int32 NY = Current.Y + DY[i];


			// マップ外
			if (!IsInside(NX, NY))
			{
				continue;
			}


			const int32 Index = NY * Width + NX;


			// すでに調べた
			if (Visited[Index])
			{
				continue;
			}


			// 壁
			if (GetTile(NX, NY) != EDungeonTile::Floor)
			{
				continue;
			}


			// 訪問済みにする
			Visited[Index] = true;

			// 次に調べる
			Queue.Enqueue(FIntPoint(NX, NY));
		}
	}


	// 訪問できた床数と
	// 全床数が同じなら全部つながっている
	return VisitedFloor == TotalFloor;
}


// ============================================================
// タイル設定
// ============================================================

void FDungeonGenerator::SetTile(int32 X, int32 Y, EDungeonTile Tile)
{
	if (!IsInside(X, Y))
	{
		return;
	}

	Tiles[Y * Width + X] = Tile;
}


// ============================================================
// タイル取得
// ============================================================

EDungeonTile FDungeonGenerator::GetTile(int32 X, int32 Y) const
{
	if (!IsInside(X, Y))
	{
		return EDungeonTile::Wall;
	}

	return Tiles[Y * Width + X];
}


// ============================================================
// 座標がマップ内か
// ============================================================

bool FDungeonGenerator::IsInside(int32 X, int32 Y) const
{
	return X >= 0 && Y >= 0 && X < Width && Y < Height;
}