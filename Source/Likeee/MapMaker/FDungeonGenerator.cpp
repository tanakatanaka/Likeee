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

void FDungeonGenerator::Generate(
	int32 InWidth,
	int32 InHeight,
	int32 Seed
)
{
	Width = InWidth;
	Height = InHeight;

	Random.Initialize(Seed);

	Blocks.Empty();
	Rooms.Empty();
	Connections.Empty();

	// á@ ëSïîï«Ç…Ç∑ÇÈ
	InitializeMap();

	// áA ãÊâÊÇçÏÇÈ
	CreateBlocks();

	// áB ïîâÆÇçÏÇÈ
	CreateRooms();

	// áC ê⁄ë±ä÷åWÇçÏÇÈ
	DecideConnections();

	// áD í òHÇçÏÇÈ
	CreateCorridors();

	// áE ê⁄ë±ämîF
	if (!CheckConnectivity())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Dungeon is not connected!")
		);
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
// É}ÉbÉvèâä˙âª
// ============================================================

void FDungeonGenerator::InitializeMap()
{
	Tiles.Init(
		EDungeonTile::Wall,
		Width * Height
	);
}


// ============================================================
// ãÊâÊê∂ê¨
// ============================================================

void FDungeonGenerator::CreateBlocks()
{
	constexpr int32 TargetBlockCount = 8;

	constexpr int32 MinBlockWidth = 10;
	constexpr int32 MinBlockHeight = 8;


	// ----------------------------------------
	// ç≈èâÇÕÉ}ÉbÉvëSëÃÇ™1ãÊâÊ
	// ----------------------------------------

	FDungeonBlock Root;

	Root.X = 1;
	Root.Y = 1;

	Root.Width = Width - 2;
	Root.Height = Height - 2;

	Blocks.Add(Root);


	// ----------------------------------------
	// ï™äÑ
	// ----------------------------------------

	int32 Attempts = 0;

	while (
		Blocks.Num() < TargetBlockCount &&
		Attempts < 200
		)
	{
		++Attempts;


		const int32 Index =
			Random.RandRange(
				0,
				Blocks.Num() - 1
			);


		const FDungeonBlock Block =
			Blocks[Index];


		// ========================================
		// ï™äÑâ¬î\Ç©
		// ========================================

		const bool bCanVertical =
			Block.Width >=
			MinBlockWidth * 2;

		const bool bCanHorizontal =
			Block.Height >=
			MinBlockHeight * 2;


		if (!bCanVertical && !bCanHorizontal)
		{
			continue;
		}


		// ========================================
		// ï™äÑï˚å¸
		// ========================================

		bool bVertical = false;


		if (bCanVertical && bCanHorizontal)
		{
			// â°í∑
			if (
				Block.Width >
				Block.Height * 1.5f
				)
			{
				bVertical = true;
			}

			// ècí∑
			else if (
				Block.Height >
				Block.Width * 1.5f
				)
			{
				bVertical = false;
			}

			// ê≥ï˚å`Ç…ãﬂÇ¢
			else
			{
				bVertical =
					Random.RandRange(0, 1) == 0;
			}
		}
		else
		{
			bVertical = bCanVertical;
		}


		// å≥ÇÃãÊâÊÇçÌèú
		Blocks.RemoveAt(Index);


		// ========================================
		// ècÇ…êÿÇÈ
		// ========================================

		if (bVertical)
		{
			const int32 Split =
				Random.RandRange(
					MinBlockWidth,
					Block.Width -
					MinBlockWidth
				);


			FDungeonBlock A;

			A.X = Block.X;
			A.Y = Block.Y;

			A.Width = Split;
			A.Height = Block.Height;


			FDungeonBlock B;

			B.X =
				Block.X + Split;

			B.Y = Block.Y;

			B.Width =
				Block.Width - Split;

			B.Height =
				Block.Height;


			Blocks.Add(A);
			Blocks.Add(B);
		}

		// ========================================
		// â°Ç…êÿÇÈ
		// ========================================

		else
		{
			const int32 Split =
				Random.RandRange(
					MinBlockHeight,
					Block.Height -
					MinBlockHeight
				);


			FDungeonBlock A;

			A.X = Block.X;
			A.Y = Block.Y;

			A.Width =
				Block.Width;

			A.Height =
				Split;


			FDungeonBlock B;

			B.X =
				Block.X;

			B.Y =
				Block.Y + Split;

			B.Width =
				Block.Width;

			B.Height =
				Block.Height - Split;


			Blocks.Add(A);
			Blocks.Add(B);
		}
	}
}


// ============================================================
// ïîâÆê∂ê¨
// ============================================================

void FDungeonGenerator::CreateRooms()
{
	constexpr int32 MinRoomWidth = 4;
	constexpr int32 MinRoomHeight = 4;

	constexpr int32 Margin = 1;


	for (FDungeonBlock& Block : Blocks)
	{
		const int32 MaxRoomWidth =
			Block.Width -
			Margin * 2;

		const int32 MaxRoomHeight =
			Block.Height -
			Margin * 2;


		if (
			MaxRoomWidth < MinRoomWidth ||
			MaxRoomHeight < MinRoomHeight
			)
		{
			continue;
		}


		// ========================================
		// ïîâÆÉTÉCÉY
		// ========================================

		const int32 RoomWidth =
			Random.RandRange(
				MinRoomWidth,
				MaxRoomWidth
			);

		const int32 RoomHeight =
			Random.RandRange(
				MinRoomHeight,
				MaxRoomHeight
			);


		// ========================================
		// ïîâÆà íu
		// ========================================

		const int32 MinX =
			Block.X + Margin;

		const int32 MaxX =
			Block.X +
			Block.Width -
			RoomWidth -
			Margin;


		const int32 MinY =
			Block.Y + Margin;

		const int32 MaxY =
			Block.Y +
			Block.Height -
			RoomHeight -
			Margin;


		const int32 RoomX =
			Random.RandRange(
				MinX,
				MaxX
			);

		const int32 RoomY =
			Random.RandRange(
				MinY,
				MaxY
			);


		// ========================================
		// Room
		// ========================================

		FDungeonRoom Room;

		Room.X = RoomX;
		Room.Y = RoomY;

		Room.Width = RoomWidth;
		Room.Height = RoomHeight;


		Block.Room = Room;

		Rooms.Add(Room);


		// ========================================
		// è∞Ç…Ç∑ÇÈ
		// ========================================

		for (
			int32 Y = Room.Y;
			Y < Room.Y + Room.Height;
			++Y
			)
		{
			for (
				int32 X = Room.X;
				X < Room.X + Room.Width;
				++X
				)
			{
				SetTile(
					X,
					Y,
					EDungeonTile::Floor
				);
			}
		}
	}
}


// ============================================================
// ïîâÆÇÃê⁄ë±
// ============================================================

void FDungeonGenerator::DecideConnections()
{
	Connections.Empty();


	if (Blocks.Num() <= 1)
	{
		return;
	}


	// åªíiäKÇ≈ÇÕ
	//
	// 0 -- 1 -- 2 -- 3 -- 4
	//
	// ÇÃÇÊÇ§Ç…ê⁄ë±

	for (
		int32 i = 0;
		i < Blocks.Num() - 1;
		++i
		)
	{
		FDungeonConnection Connection;

		Connection.A = i;
		Connection.B = i + 1;

		Connections.Add(Connection);
	}
}


// ============================================================
// í òHê∂ê¨
// ============================================================

void FDungeonGenerator::CreateCorridors()
{
	for (
		const FDungeonConnection& Connection :
		Connections
		)
	{
		const FDungeonRoom& RoomA =
			Blocks[Connection.A].Room;

		const FDungeonRoom& RoomB =
			Blocks[Connection.B].Room;


		const int32 X1 =
			RoomA.CenterX();

		const int32 Y1 =
			RoomA.CenterY();


		const int32 X2 =
			RoomB.CenterX();

		const int32 Y2 =
			RoomB.CenterY();


		// ========================================
		// Léöí òH
		// ========================================

		if (
			Random.RandRange(0, 1) == 0
			)
		{
			DigHorizontal(
				X1,
				X2,
				Y1
			);

			DigVertical(
				Y1,
				Y2,
				X2
			);
		}
		else
		{
			DigVertical(
				Y1,
				Y2,
				X1
			);

			DigHorizontal(
				X1,
				X2,
				Y2
			);
		}
	}
}


// ============================================================
// â°í òH
// ============================================================

void FDungeonGenerator::DigHorizontal(
	int32 X1,
	int32 X2,
	int32 Y
)
{
	if (X1 > X2)
	{
		Swap(X1, X2);
	}


	for (
		int32 X = X1;
		X <= X2;
		++X
		)
	{
		SetTile(
			X,
			Y,
			EDungeonTile::Floor
		);
	}
}


// ============================================================
// ècí òH
// ============================================================

void FDungeonGenerator::DigVertical(
	int32 Y1,
	int32 Y2,
	int32 X
)
{
	if (Y1 > Y2)
	{
		Swap(Y1, Y2);
	}


	for (
		int32 Y = Y1;
		Y <= Y2;
		++Y
		)
	{
		SetTile(
			X,
			Y,
			EDungeonTile::Floor
		);
	}
}


// ============================================================
// BFS ê⁄ë±É`ÉFÉbÉN
// ============================================================

bool FDungeonGenerator::CheckConnectivity() const
{
	int32 StartX = -1;
	int32 StartY = -1;

	int32 TotalFloor = 0;


	// ========================================
	// è∞ÇíTÇ∑
	// ========================================

	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			if (
				GetTile(X, Y) ==
				EDungeonTile::Floor
				)
			{
				++TotalFloor;


				if (StartX == -1)
				{
					StartX = X;
					StartY = Y;
				}
			}
		}
	}


	if (StartX == -1)
	{
		return false;
	}


	// ========================================
	// ñKñ‚çœÇ›
	// ========================================

	TArray<bool> Visited;

	Visited.Init(
		false,
		Width * Height
	);


	TQueue<FIntPoint> Queue;


	Queue.Enqueue(
		FIntPoint(
			StartX,
			StartY
		)
	);


	Visited[
		StartY * Width + StartX
	] = true;


	static constexpr int32 DX[4] =
	{
		1, -1, 0, 0
	};

	static constexpr int32 DY[4] =
	{
		0, 0, 1, -1
	};


	int32 VisitedFloor = 0;


	// ========================================
	// BFS
	// ========================================

	while (!Queue.IsEmpty())
	{
		FIntPoint Current;

		Queue.Dequeue(Current);

		++VisitedFloor;


		for (int32 i = 0; i < 4; ++i)
		{
			const int32 NX =
				Current.X + DX[i];

			const int32 NY =
				Current.Y + DY[i];


			if (!IsInside(NX, NY))
			{
				continue;
			}


			const int32 Index =
				NY * Width + NX;


			if (Visited[Index])
			{
				continue;
			}


			if (
				GetTile(NX, NY) !=
				EDungeonTile::Floor
				)
			{
				continue;
			}


			Visited[Index] = true;


			Queue.Enqueue(
				FIntPoint(
					NX,
					NY
				)
			);
		}
	}


	return
		VisitedFloor ==
		TotalFloor;
}


// ============================================================
// Tileê›íË
// ============================================================

void FDungeonGenerator::SetTile(
	int32 X,
	int32 Y,
	EDungeonTile Tile
)
{
	if (!IsInside(X, Y))
	{
		return;
	}


	Tiles[
		Y * Width + X
	] = Tile;
}


// ============================================================
// TileéÊìæ
// ============================================================

EDungeonTile FDungeonGenerator::GetTile(
	int32 X,
	int32 Y
) const
{
	if (!IsInside(X, Y))
	{
		return EDungeonTile::Wall;
	}


	return Tiles[
		Y * Width + X
	];
}


// ============================================================
// îÕàÕì‡Ç©
// ============================================================

bool FDungeonGenerator::IsInside(
	int32 X,
	int32 Y
) const
{
	return
		X >= 0 &&
		Y >= 0 &&
		X < Width &&
		Y < Height;
}