#pragma once

#include "AI.h"


class EasyAI : public AI
{
private:
	static const int movePercent = 45;
	static const int breakPercent = 55;

	static const int maxWaitTimer = 30;

private:
	bool startMoveFlg;
	int moveCnt;

public:
	 EasyAI();
	~EasyAI();

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
};