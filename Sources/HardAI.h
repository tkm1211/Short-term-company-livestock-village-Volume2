#pragma once

#include "AI.h"
#include "Block.h"


struct BreakBlock
{
	Block* block;
	int breakPoint;
};

struct CheckBlock
{
	Block* originBlock;
	Block* tipBlock;
	BreakBlock breakBlock;
};

class HardAI : public AI
{
private:
	static const int movePercent = 45;
	static const int breakPercent = 55;

	static const int maxWaitTimer = 2;

private:
	bool startMoveFlg;
	int moveCnt;

	bool searched; // î’ñ íTçıçœÇ›ÅH
	int goalRow;
	int goalColumn;

public:
	 HardAI();
	~HardAI();

public:
	void Init();
	void Update(int row, int column);

	void ChangeAIStatus();
	void ChangeAIStatus(int row, int column);
	void WaitUpdate();
	int  CheckMove();
	int  CheckMove(int row, int column, int goalRow, int goalColumn);
	void MoveUpdate();
	void MoveUpdate(int row, int column);
	void BreakUpdate();

private:
	void StartMove();
	void CheckBlockToBreak(int& row, int& column);
};