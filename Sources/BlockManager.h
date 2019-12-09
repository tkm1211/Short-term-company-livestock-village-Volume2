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
	static int constexpr BLOCK_NUM_MAX = ROW_MAX * COLUMN_MAX;

	static int constexpr BOARD_ROW_MAX = 6;
	static int constexpr BOARD_COLUMN_MAX = 9;
	static int constexpr BLOCK_BOARD_MAX = BOARD_ROW_MAX * BOARD_COLUMN_MAX;

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

	std::array<Block, BLOCK_NUM_MAX> block;
	//std::array<std::array<Block, COLUMN_MAX - 10>, ROW_MAX> sortBlocks; //ゲームの盤面のみ
	Block sortBlocks[COLUMN_MAX - 10][ROW_MAX];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ブロックの生成系
	void GenerateBlock(int _row, int _column, Color _color);
	void SetStartBlock();
	// ブロックの破壊系
	void BreakBlock(int _row, int _column);
	void CheckDownBlock();
	// ブロックのソート関数
	void SetSortBlocks();
	// ブロックのコンボ系
	void RagisterChainBlock();
};

extern BlockManager provisionalBlockManager;