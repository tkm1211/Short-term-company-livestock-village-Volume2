#pragma once

#include <memory>
#include <array>

#include "sprite.h"
#include "Block.h"

class BlockManager
{
public:
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9 + 9 + 1; // 盤面 + お邪魔が上に上限一面分 + 下の次に出てくるブロック
	static int constexpr BLOCK_MAX = ROW_MAX * COLUMN_MAX;

	static constexpr int ADJUST = 3;
	static constexpr int SINGLE_CORRECTION_X = 615;
	static constexpr int SINGLE_CORRECTION_Y = 12;

public:
	enum Color
	{
		Red,
		LightBlue,
		Yellow,
		Green,
		Orange,
		Blue,
		Purple,
		Obstacle,
	};

private:
	std::shared_ptr<SpriteBatch> sprBlock;

	std::array<Block, BLOCK_MAX> block;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void GenerateBlock(int _row, int _column, Color _color);
	void SetStartBlock();
};

extern BlockManager provisionalBlockManager;