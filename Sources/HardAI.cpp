#include "HardAI.h"
#include "BlockManager.h"


HardAI::HardAI()
{
}

HardAI::~HardAI()
{
}

void HardAI::Init()
{
	aiStatus = AIStatus::Wait;
	debugAiStatus = AIStatus::Wait;
	inputFlg = flgInit;
	nextStatusTimer = 0;
	waitTimer = 0;
	startMoveFlg = true;
	moveCnt = 0;
	searched = false;
	goalRow = -1;
	goalColumn = -1;
}

void HardAI::Update(int row, int column)
{
	inputFlg = flgInit;

	if (startMoveFlg)
	{
		StartMove();
		return;
	}

	ChangeAIStatus(row, column);

	switch (aiStatus)
	{
	case AIStatus::Wait:
		WaitUpdate();
		break;
	case AIStatus::Move:
		MoveUpdate(row, column);
		break;
	case AIStatus::Break:
		BreakUpdate();
		break;
	default: break;
	}
}

void HardAI::StartMove()
{
	if (aiStatus == AIStatus::Change)
	{
		aiStatus = AIStatus::Move;
	}
	else if (aiStatus == AIStatus::Wait)
	{
		WaitUpdate();
	}
	else if (aiStatus == AIStatus::Move)
	{
		inputFlg |= downInput;
		aiStatus = AIStatus::Wait;
		moveCnt++;
	}

	if (4 <= moveCnt)
	{
		startMoveFlg = false;
		moveCnt = 0;
		aiStatus = AIStatus::Wait;
	}
}

void HardAI::ChangeAIStatus()
{
}

void HardAI::ChangeAIStatus(int row, int column)
{
	if (aiStatus != AIStatus::Change) return;

	if (!searched)
	{
		CheckBlockToBreak(goalRow, goalColumn);
	}

	if (goalRow == -1 || goalColumn == -1)
	{
		aiStatus = AIStatus::Wait;
	}
	else if (row == goalRow && column == goalColumn)
	{
		searched = false;
		aiStatus = AIStatus::Break;
	}
	else
	{
		aiStatus = AIStatus::Move;
	}
}

void HardAI::CheckBlockToBreak(int& row, int& column)
{
	searched = true;

	int cheakBlocksNum = 0;

	std::array<CheckBlock, BlockManager::BLOCK_BOARD_MAX * 3> cheakBlocks;

	Block* block;
	Block* upBlock;
	Block* downBlock;
	Block* leftBlock;
	Block* rightBlock;

	auto CheckUp = [&](int row, int column)
	{
		if (block->GetColor() == upBlock->GetColor())
		{
			regularBlockManager[1].SearchAliveBlock(row, column - 1, &downBlock);

			BreakBlock breakBlock;
			breakBlock.block = downBlock;
			breakBlock.breakPoint = 1;

			CheckBlock checkBlock;
			checkBlock.originBlock = block;
			checkBlock.tipBlock = upBlock;
			checkBlock.breakBlock = breakBlock;
			cheakBlocks[cheakBlocksNum++] = checkBlock;
			return true;
		}

		return false;
	};
	auto CheckLeft = [&](int row, int column)
	{
		if (block->GetColor() == leftBlock->GetColor())
		{
			regularBlockManager[1].SearchAliveBlock(row - 1 , column, &downBlock);

			BreakBlock breakBlock;
			breakBlock.block = downBlock;
			breakBlock.breakPoint = 1;

			CheckBlock checkBlock;
			checkBlock.originBlock = block;
			checkBlock.tipBlock = leftBlock;
			checkBlock.breakBlock = breakBlock;
			cheakBlocks[cheakBlocksNum++] = checkBlock;
			return true;
		}

		return false;
	};
	auto CheckRight = [&](int row, int column)
	{
		if (block->GetColor() == rightBlock->GetColor())
		{
			regularBlockManager[1].SearchAliveBlock(row + 1, column, &downBlock);

			BreakBlock breakBlock;
			breakBlock.block = downBlock;
			breakBlock.breakPoint = 1;

			CheckBlock checkBlock;
			checkBlock.originBlock = block;
			checkBlock.tipBlock = rightBlock;
			checkBlock.breakBlock = breakBlock;
			cheakBlocks[cheakBlocksNum++] = checkBlock;
			return true;
		}

		return false;
	};

	// 全てのブレイクポイントを探索
	{
		for (int j = BlockManager::BOARD_COLUMN_MAX - 1; 0 <= j; j--)
		{
			for (int i = 0; i < BlockManager::BOARD_ROW_MAX; i++)
			{
				block = nullptr;
				regularBlockManager[1].SearchAliveBlock(i, j, &block);

				if (block)
				{
					upBlock = nullptr;
					downBlock = nullptr;
					leftBlock = nullptr;
					rightBlock = nullptr;

					if (i == 0)
					{
						if (j == BlockManager::BOARD_COLUMN_MAX - 2)
						{
							regularBlockManager[1].SearchAliveBlock(i, j - 2, &upBlock);
							if (upBlock) CheckUp(i, j);
						}

						regularBlockManager[1].SearchAliveBlock(i + 1, j - 1, &rightBlock);
						if (rightBlock) CheckRight(i, j);
					}
					else if (i == BlockManager::BOARD_ROW_MAX - 1)
					{
						if (j == BlockManager::BOARD_COLUMN_MAX - 2)
						{
							regularBlockManager[1].SearchAliveBlock(i, j - 2, &upBlock);
							if (upBlock) CheckUp(i, j);
						}

						regularBlockManager[1].SearchAliveBlock(i - 1, j - 1, &leftBlock);
						if (leftBlock) CheckLeft(i, j);
					}
					else
					{
						if (j == BlockManager::BOARD_COLUMN_MAX - 2)
						{
							regularBlockManager[1].SearchAliveBlock(i, j - 2, &upBlock);
							if (upBlock) CheckUp(i, j);
						}

						regularBlockManager[1].SearchAliveBlock(i - 1, j - 1, &leftBlock);
						regularBlockManager[1].SearchAliveBlock(i + 1, j - 1, &rightBlock);
						if (leftBlock) CheckLeft(i, j);
						if (rightBlock) CheckRight(i, j);
					}
				}
			}
		}
	}

	// ブレイクポイントを集約
	{
		// チェックブロックからブレイクポイントを取得し集める
		for (int i = 0; i < cheakBlocksNum; i++)
		{
			for (int j = i + 1; j < cheakBlocksNum; j++)
			{
				// 念のためにブレイクポイントが１以上あるか？
				if (cheakBlocks[i].breakBlock.breakPoint < 1)
				{
					continue;
				}

				// 原点のブロックが同じではないか？
				if (cheakBlocks[i].originBlock->GetRow() == cheakBlocks[j].originBlock->GetRow() && cheakBlocks[i].originBlock->GetColumn() == cheakBlocks[j].originBlock->GetColumn())
				{
					continue;
				}

				// 上下に挟まっていないか？
				if (cheakBlocks[i].breakBlock.block->GetRow() == cheakBlocks[j].tipBlock->GetRow() && cheakBlocks[i].breakBlock.block->GetColumn() == cheakBlocks[j].tipBlock->GetColumn())
				{
					continue;
				}

				// 右に流れている状態
				if (cheakBlocks[i].breakBlock.block->GetRow() == cheakBlocks[j].originBlock->GetRow() && cheakBlocks[i].breakBlock.block->GetColumn() == cheakBlocks[j].originBlock->GetColumn())
				{
					cheakBlocks[j].breakBlock.breakPoint += cheakBlocks[i].breakBlock.breakPoint;
					//cheakBlocks[j].breakBlock.breakPoint++;
					//cheakBlocks[i].breakBlock.breakPoint--;
				}
				// 左に流れている状態
				else if (cheakBlocks[i].originBlock->GetRow() == cheakBlocks[j].breakBlock.block->GetRow() && cheakBlocks[i].originBlock->GetColumn() == cheakBlocks[j].breakBlock.block->GetColumn())
				{
					cheakBlocks[i].breakBlock.breakPoint += cheakBlocks[j].breakBlock.breakPoint;
					//cheakBlocks[i].breakBlock.breakPoint++;
					//cheakBlocks[j].breakBlock.breakPoint--;
				}
			}
		}
	}

	// ブレイクポイント数が多いブロックを検索
	int hitIndex = -1;
	{
		int breakPoint = 0;
		for (int i = 0; i < cheakBlocksNum/*BlockManager::BLOCK_BOARD_MAX * 3*/; i++)
		{
			if (breakPoint < cheakBlocks[i].breakBlock.breakPoint)
			{
				breakPoint = cheakBlocks[i].breakBlock.breakPoint;
				hitIndex = i;
			}
		}
	}

	// 破壊するブロックの位置を取得
	{
		if (0 <= hitIndex)
		{
			row = cheakBlocks[hitIndex].breakBlock.block->GetRow();
			column = cheakBlocks[hitIndex].breakBlock.block->GetColumn();
		}
	}
}

void HardAI::WaitUpdate()
{
	if (maxWaitTimer < waitTimer++)
	{
		aiStatus = AIStatus::Change;
		waitTimer = 0;
	}
}

int HardAI::CheckMove()
{
	return 0;
}

int HardAI::CheckMove(int row, int column, int goalRow, int goalColumn)
{
	if (goalColumn < column)
	{
		return 0;
	}
	else if (column < goalColumn)
	{
		return 1;
	}
	else if (goalRow < row)
	{
		return 2;
	}
	else if (row < goalRow)
	{
		return 3;
	}

	return -1;
}

void HardAI::MoveUpdate()
{
}

void HardAI::MoveUpdate(int row, int column)
{
	aiStatus = AIStatus::Wait;

	switch (CheckMove(row, column, goalRow, goalColumn))
	{
	case 0: // UP
		inputFlg |= upInput;
		break;
	case 1: // DOWN
		inputFlg |= downInput;
		break;
	case 2: // LEFT
		inputFlg |= leftInput;
		break;
	case 3: // RIGHT
		inputFlg |= rightInput;
		break;
	default: break;
	}
}

void HardAI::BreakUpdate()
{
	aiStatus = AIStatus::Change;
	inputFlg |= breakInput;
}