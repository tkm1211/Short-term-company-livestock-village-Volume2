#pragma once

#include "AI.h"


class NormalAI : public AI
{
private:
	static const int movePercent = 45;
	static const int breakPercent = 55;

	static const int StackedBlocksTopColumn = 3;

	static const int maxWaitTimer = 15;

private:
	bool startMoveFlg;
	int moveCnt;

	// StackedBlocksBreak
	bool stackedBlocksBreak;
	int topBlockRow;
	int topBlockColumn;

public:
	 NormalAI();
	~NormalAI();

public:
	void Init();
	void Update(int row, int column);

	void ChangeAIStatus();
	void WaitUpdate();
	int  CheckMove();
	void MoveUpdate();
	void BreakUpdate();

private:
	void StartMove();
	bool SearchStackedBlocks();
	void StackedBlocksBreakUpdate(int row, int column);
};