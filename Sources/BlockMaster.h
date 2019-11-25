#pragma once

#include "Singleton.h"
#include "BlockManager.h"

#define COLOR_MAX	4

class BlockMaster : public Singleton<BlockMaster>
{
	enum COLOR
	{
		RED,
		GREEN,
		BLUE,
		PURPLE,
	};

public:
	BlockManager blockManager[2];

	static int constexpr BLOCK_MAX = 54;
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
#define BLOCK_MASTER BlockMaster::getInstance()