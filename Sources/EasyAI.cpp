#include <memory>

#include "EasyAI.h"


EasyAI::EasyAI()
{
}

EasyAI::~EasyAI()
{
}

void EasyAI::Init()
{
	aiStatus = AIStatus::Wait;
	debugAiStatus = AIStatus::Wait;
	inputFlg = flgInit;
	nextStatusTimer = 0;
	waitTimer = 0;
	startMoveFlg = true;
	moveCnt = 0;
}

void EasyAI::Update(int row, int column)
{
	inputFlg = flgInit;

	if (startMoveFlg)
	{
		StartMove();
		return;
	}

	ChangeAIStatus();

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

void EasyAI::StartMove()
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

void EasyAI::ChangeAIStatus()
{
	if (aiStatus != AIStatus::Change) return;


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

void EasyAI::WaitUpdate()
{
	if (maxWaitTimer < waitTimer++)
	{
		aiStatus = AIStatus::Change;
		waitTimer = 0;
	}
}

int EasyAI::CheckMove()
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

void EasyAI::MoveUpdate()
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

void EasyAI::BreakUpdate()
{
	aiStatus = AIStatus::Wait;

	inputFlg |= breakInput;
}