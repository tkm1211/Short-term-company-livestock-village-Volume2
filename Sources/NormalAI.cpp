#include <memory>

#include "NormalAI.h"
#include "BlockManager.h"


NormalAI::NormalAI()
{
}

NormalAI::~NormalAI()
{
}

void NormalAI::Init()
{
	aiStatus = AIStatus::Wait;
	debugAiStatus = AIStatus::Wait;
	inputFlg = flgInit;
	nextStatusTimer = 0;
	waitTimer = 0;
	startMoveFlg = true;
	moveCnt = 0;

	 stackedBlocksBreak = false;
	 topBlockRow = -1;
	 topBlockColumn = -1;
}

void NormalAI::Update(int row, int column)
{
	inputFlg = flgInit;

	if (startMoveFlg)
	{
		StartMove();
		return;
	}

	ChangeAIStatus();
	StackedBlocksBreakUpdate(row, column);

	switch (aiStatus)
	{
	case AIStatus::Wait:
		WaitUpdate();
		break;
	case AIStatus::Move:
		MoveUpdate();
		break;
	case AIStatus::Break:
		BreakUpdate();
		break;
	default: break;
	}
}

void NormalAI::StackedBlocksBreakUpdate(int row, int column)
{
	if (stackedBlocksBreak)
	{
		if (aiStatus == AIStatus::Change)
		{
			aiStatus = AIStatus::Wait;

			if (topBlockRow == row)
			{
				BreakUpdate();
				stackedBlocksBreak = false;
				topBlockRow = -1;
				topBlockColumn = -1;
			}
			else if (row < topBlockRow)
			{
				inputFlg |= rightInput;
			}
			else if (topBlockRow < row)
			{
				inputFlg |= leftInput;
			}
		}
	}
}

void NormalAI::StartMove()
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

void NormalAI::ChangeAIStatus()
{
	if (aiStatus != AIStatus::Change) return;
	if (stackedBlocksBreak) return;
	if (SearchStackedBlocks()) return;


	int randPercent = rand() % 100;

	if (randPercent < movePercent)
	{
		aiStatus = AIStatus::Move;
		debugAiStatus = AIStatus::Move;
	}
	else if (randPercent < movePercent + breakPercent)
	{
		aiStatus = AIStatus::Break;
		debugAiStatus = AIStatus::Break;
	}
}

bool NormalAI::SearchStackedBlocks()
{
	stackedBlocksBreak = false;
	topBlockColumn = 10;

	for (auto blocks : regularBlockManager[1].GetBlocks())
	{
		int comlumn = blocks.GetColumn();

		if (comlumn < 0) continue;
		if (StackedBlocksTopColumn <= comlumn) continue;
		if (topBlockColumn <= comlumn) continue;

		stackedBlocksBreak = true;
		topBlockRow = blocks.GetRow();
		topBlockColumn = comlumn;
	}

	return stackedBlocksBreak;
}

void NormalAI::WaitUpdate()
{
	if (maxWaitTimer < waitTimer++)
	{
		aiStatus = AIStatus::Change;
		waitTimer = 0;
	}
}

int NormalAI::CheckMove()
{
	int randPercent = rand() % 100;

	if (randPercent < 10)
	{
		return 0;
	}
	else if (randPercent < 20)
	{
		return 1;
	}
	else if (randPercent < 65)
	{
		return 2;
	}
	else if (randPercent < 100)
	{
		return 3;
	}

	return -1;
}

void NormalAI::MoveUpdate()
{
	aiStatus = AIStatus::Wait;


	switch (CheckMove())
	{
	case 0: // UP
		inputFlg |= upInput;
		break;
	case 1: // DOWN
		inputFlg |= downInput;
		break;
	case 2: // RIGHT
		inputFlg |= rightInput;
		break;
	case 3: // LEFT
		inputFlg |= leftInput;
		break;
	default: break;
	}
}

void NormalAI::BreakUpdate()
{
	aiStatus = AIStatus::Wait;

	inputFlg |= breakInput;
}