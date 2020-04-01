#include "AI.h"


AIStatus AI::GetAIStatus()
{
	return aiStatus;
}

AIStatus AI::GetDebugAIStatus()
{
	return debugAiStatus;
}

bool AI::GetInput(AIInput _input)
{
	switch (_input)
	{
	case AIInput::BreakInput:
		if (inputFlg & breakInput)
		{
			return true;
		}
		break;
	case AIInput::UpInput:
		if (inputFlg & upInput)
		{
			return true;
		}
		break;
	case AIInput::DownInput:
		if (inputFlg & downInput)
		{
			return true;
		}
		break;
	case AIInput::RightInput:
		if (inputFlg & rightInput)
		{
			return true;
		}
		break;
	case AIInput::LeftInput:
		if (inputFlg & leftInput)
		{
			return true;
		}
		break;
	default: break;
	}


	return false;
}