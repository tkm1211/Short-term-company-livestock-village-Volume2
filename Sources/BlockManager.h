#pragma once

#include <memory>
#include <array>

#include "sprite.h"
#include "Block.h"

class BlockManager
{
public:
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9 + 9 + 1; // �Ֆ� + ���ז�����ɏ����ʕ� + ���̎��ɏo�Ă���u���b�N
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
	//std::array<std::array<Block, COLUMN_MAX - 10>, ROW_MAX> sortBlocks; //�Q�[���̔Ֆʂ̂�
	Block sortBlocks[COLUMN_MAX - 10][ROW_MAX];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �u���b�N�̐����n
	void GenerateBlock(int _row, int _column, Color _color);
	void SetStartBlock();
	// �u���b�N�̔j��n
	void BreakBlock(int _row, int _column);
	void CheckDownBlock();
	// �u���b�N�̃\�[�g�֐�
	void SetSortBlocks();
	// �u���b�N�̃R���{�n
	void RagisterChainBlock();
};

extern BlockManager provisionalBlockManager;