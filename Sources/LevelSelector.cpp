#include "LevelSelector.h"
#include "CharacterSelector.h"
#include "Resource.h"
#include "input_device.h"


void LevelSelector::Construct()
{
	sprSelect = RESOURCE->GetSpriteData(Resource::Texture::Select);
	sprSelectCPU = RESOURCE->GetSpriteData(Resource::Texture::SelectCPU);
	sprCharacters[0] = RESOURCE->GetSpriteData(Resource::Texture::Character01);
	sprCharacters[1] = RESOURCE->GetSpriteData(Resource::Texture::Character02);
	sprCharacters[2] = RESOURCE->GetSpriteData(Resource::Texture::Character03);
	sprCharacters[3] = RESOURCE->GetSpriteData(Resource::Texture::Character04);
	sprCharacters[4] = RESOURCE->GetSpriteData(Resource::Texture::Character05);
	sprCharacters[5] = RESOURCE->GetSpriteData(Resource::Texture::Character06);
	sprCharacters[6] = RESOURCE->GetSpriteData(Resource::Texture::Character07);
	sprCharacters[7] = RESOURCE->GetSpriteData(Resource::Texture::Character08);
}

void LevelSelector::Init(ISelector* selector)
{
	CharacterSelector* characterSelector = static_cast<CharacterSelector*>(selector);
	gameMode = characterSelector->GetSelectGameMode();
	selectMode = SelectMode::CharacterSelect;
	thisSelectMode = SelectMode::CharacterSelect;
	updateState = UpdateState::BeginMove;

	switch (gameMode)
	{
	case SelectGameMode::Single:
		pos[0] = DirectX::XMFLOAT2(levelPosX, levelSinglePosY);
		controllerCnt = 1;
		break;

	case SelectGameMode::CPU:
	case SelectGameMode::Multi:
		pos[0] = DirectX::XMFLOAT2(levelPosX, levelMulti01PosY);
		pos[1] = DirectX::XMFLOAT2(levelPosX, levelMulti02PosY);
		controllerCnt = 2;
		break;

	default: break;
	}

	cpuPoworPos = DirectX::XMFLOAT2(cpuPoworPosX, cpuPoworPosY);

	characterPosX[0] = characterLeftPosX;
	characterPosX[1] = characterRightPosX;

	levelNum[0] = levelDefault;
	levelNum[1] = levelDefault;
	decision[0] = false;
	decision[1] = false;
	charNum[0] = characterSelector->GetSelectCharacterNum(0);
	charNum[1] = characterSelector->GetSelectCharacterNum(1);

	cpuPoworNum = cpuPoworDefault;
	cpuPoworDecision = false;

	moveCnt = 0;

	okPos[0] = DirectX::XMFLOAT2(okLeftPosX, okPosY);
	okPos[1] = DirectX::XMFLOAT2(okRightPosX, okPosY);
	for (int i = 0; i < 2; i++)
	{
		okMoveCnt[i] = 0;
		okAlpha[i] = 1.0f;
		okAlphaFlg[i] = false;
		okBegin[i] = false;
		okEnd[i] = false;
	}
}

bool LevelSelector::Update()
{
	bool ch = false;

	switch (updateState)
	{
	case UpdateState::BeginMove:
		BeginMove();
		break;
	case UpdateState::Choice:
		Choice();
		break;
	case UpdateState::EndMove:
		ch = EndMove();
		break;
	default: break;
	}

	return ch;
}

void LevelSelector::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(240.0f, 72.0f, 1440.0f, 160.0f, 0.0f, 320.0f, 1440.0f, 160.0f, 0.0f, 0.0f, 0.0f);
	sprSelect->End();

	for (int i = 0; i < controllerCnt; i++)
	{
		if (gameMode == SelectGameMode::CPU)
		{
			sprSelectCPU->Begin();
			sprSelectCPU->Draw(pos[i].x, pos[i].y, levelSizeX, levelSizeY, levelSizeX * (levelNum[i] - 1), levelTexPosY + levelSizeY * i, levelSizeX, levelSizeY, 0.0f, 0.0f, 0.0f);
			if (decision[i]) sprSelectCPU->Draw(pos[i].x, pos[i].y, levelSizeX, levelSizeY, levelSelectedTexPosX, levelSelectedTexPosY + levelSizeY * i, levelSizeX, levelSizeY, 0.0f, 0.0f, 0.0f);
			if (i == 1)
			{
				sprSelectCPU->Draw(cpuPoworPos.x, cpuPoworPos.y, cpuPoworSizeX, cpuPoworSizeY, cpuPoworSizeX * (cpuPoworNum - 1), cpuPoworTexPosY, cpuPoworSizeX, cpuPoworSizeY, 0.0f, 0.0f, 0.0f);
				if (cpuPoworDecision) sprSelectCPU->Draw(cpuPoworPos.x, cpuPoworPos.y, cpuPoworSizeX, cpuPoworSizeY, cpuPoworSelectedTexPosX, cpuPoworSelectedTexPosY, cpuPoworSizeX, cpuPoworSizeY, 0.0f, 0.0f, 0.0f);
			}
			if (updateState == UpdateState::Choice) sprSelectCPU->Draw(okPos[i].x, okPos[i].y, okSizeX, okSizeY, okTexPosX + okSizeX * i, okTexPosY, okSizeX, okSizeY, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, okAlpha[i]);
			sprSelectCPU->End();
		}
		else
		{
			sprSelect->Begin();
			sprSelect->Draw(pos[i].x, pos[i].y, levelSizeX, levelSizeY, levelSizeX * (levelNum[i] - 1), levelTexPosY + levelSizeY * i, levelSizeX, levelSizeY, 0.0f, 0.0f, 0.0f);
			if (decision[i]) sprSelect->Draw(pos[i].x, pos[i].y, levelSizeX, levelSizeY, levelSelectedTexPosX, levelSelectedTexPosY + levelSizeY * i, levelSizeX, levelSizeY, 0.0f, 0.0f, 0.0f);
			if (updateState == UpdateState::Choice)
			{
				sprSelect->Draw(okPos[i].x, okPos[i].y, okSizeX, okSizeY, okTexPosX + okSizeX * i, okTexPosY, okSizeX, okSizeY, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, okAlpha[i]);
			}
			sprSelect->End();
		}

		sprCharacters[charNum[i]]->Begin();
		sprCharacters[charNum[i]]->Draw(characterPosX[i], characterPosY, i == 0 ? characterSizeX : -characterSizeX, characterSizeY, 0.0f, 0.0f, characterSizeX, characterSizeY, 0.0f, 0.0f, 0.0f);
		sprCharacters[charNum[i]]->End();
	}
}

void LevelSelector::BeginMove()
{
	if (moveCntMax <= moveCnt++)
	{
		moveCnt = 0;
		updateState = UpdateState::Choice;
		return;
	}

	/*switch (gameMode)
	{
	case SelectGameMode::Single:
		pos[0].y -= singleMoveSpeedY;
		break;

	case SelectGameMode::CPU:
		cpuPoworPos.y -= multiMoveSpeedY;

	case SelectGameMode::Multi:
		pos[0].y -= multiMoveSpeedY;
		pos[1].y -= multiMoveSpeedY;
		break;

	default: break;
	}*/

	switch (gameMode)
	{
	case SelectGameMode::Single:
		pos[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 508.0f, 1080.0f);
		break;

	case SelectGameMode::CPU:
		cpuPoworPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 912.0f, cpuPoworPosY);

	case SelectGameMode::Multi:
		pos[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 330.0f, levelMulti01PosY);
		pos[1].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 654.0f, levelMulti02PosY);
		break;

	default: break;
	}
}

bool LevelSelector::EndMove()
{
	if (selectMode == SelectMode::GoGame) return true;

	if (moveCntMax <= moveCnt++)
	{
		moveCnt = 0;
		return true;
	}

	/*switch (gameMode)
	{
	case SelectGameMode::Single:
		pos[0].y += singleMoveSpeedY;
		break;

	case SelectGameMode::CPU:
		cpuPoworPos.y += multiMoveSpeedY;
		characterPosX[1] += cpuCharacterMoveSpeedX;
		
	case SelectGameMode::Multi:
		pos[0].y += multiMoveSpeedY;
		pos[1].y += multiMoveSpeedY;
		break;

	default: break;
	}*/

	switch (gameMode)
	{
	case SelectGameMode::Single:
		pos[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 1080.0f, 508.0f);
		break;

	case SelectGameMode::CPU:
		cpuPoworPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), cpuPoworPosY, 912.0f);
		characterPosX[1] = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 2336.0f, characterRightPosX);

	case SelectGameMode::Multi:
		pos[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), levelMulti01PosY, 330.0f);
		pos[1].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), levelMulti02PosY, 654.0f);
		break;

	default: break;
	}

	return false;
}

void LevelSelector::Choice()
{
	if (pad[0].bBt && !decision[0] && !decision[1] && !okEnd[0] && !okEnd[1])
	{
		updateState = UpdateState::EndMove;
		selectMode = SelectMode::CharacterSelect;
	}
	else
	{
		if (gameMode == SelectGameMode::Single && decision[0] && okEnd[0])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::GoGame;
		}
		else if (gameMode == SelectGameMode::Multi && decision[0] && decision[1] && okEnd[0] && okEnd[1])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::GoGame;
		}
		else if (gameMode == SelectGameMode::CPU && decision[0] && decision[1] && cpuPoworDecision && okEnd[0] && okEnd[1])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::GoGame;
		}
	}

	OkMove();
	Operation();
}

void LevelSelector::OkMove()
{
	switch (gameMode)
	{
	case SelectGameMode::Single:
		if (okAlphaFlg[0])
		{
			if (okWaitCntMax + okAlphaCntMax <= okMoveCnt[0])
			{
				okMoveCnt[0] = 0;
				okEnd[0] = okBegin[0];
				okBegin[0] = false;
			}
			if (okWaitCntMax <= okMoveCnt[0])
			{
				okAlpha[0] -= okAlphaSpeedY;
				//okAlpha[0] = easing::OutExp(static_cast<float>(okMoveCnt[0]), static_cast<float>(okMoveCntMax), 0.0f, 1.0f);
			}

			okMoveCnt[0]++;
		}
		else if (okBegin[0])
		{
			if (okMoveCntMax <= okMoveCnt[0])
			{
				okMoveCnt[0] = 0;
				okAlphaFlg[0] = true;
			}
			else
			{
				//okPos[0].y -= okMoveSpeedY;
				okPos[0].y = easing::OutExp(static_cast<float>(okMoveCnt[0]), static_cast<float>(okMoveCntMax), okGoalPosY, okPosY);
			}

			okMoveCnt[0]++;
		}
		break;

	case SelectGameMode::CPU:
		for (int i = 0; i < 2; i++)
		{
			if (okAlphaFlg[0] && okAlphaFlg[1] && decision[0] && decision[1] && cpuPoworDecision)
			{
				if (okWaitCntMax + okAlphaCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okEnd[i] = okBegin[i];
					okBegin[i] = false;
				}
				if (okWaitCntMax <= okMoveCnt[i])
				{
					okAlpha[i] -= okAlphaSpeedY;
					//okAlpha[i] = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okWaitCntMax), 0.0f, 1.0f);
				}

				okMoveCnt[i]++;
			}
			else if (okAlphaFlg[i] && !decision[i])
			{
				if (okWaitCntMax + okAlphaCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okBegin[i] = false;
					okEnd[i] = false;

					okPos[0] = DirectX::XMFLOAT2(okLeftPosX, okPosY);
					okAlpha[0] = 1.0f;

					okAlphaFlg[i] = false;
				}
				if (okWaitCntMax <= okMoveCnt[i])
				{
					okAlpha[i] -= okAlphaSpeedY;
					//okAlpha[i] = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okWaitCntMax), 0.0f, 1.0f);
				}

				okMoveCnt[i]++;
			}
			else if (okBegin[i] && !okAlphaFlg[i])
			{
				if (i == 1 && !cpuPoworDecision) continue;
				if (okMoveCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okAlphaFlg[i] = true;
				}
				else
				{
					//okPos[i].y -= okMoveSpeedY;
					okPos[i].y = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okMoveCntMax), okGoalPosY, okPosY);
				}

				okMoveCnt[i]++;
			}
		}
		break;

	case SelectGameMode::Multi:
		for (int i = 0; i < 2; i++)
		{
			if (okAlphaFlg[0] && okAlphaFlg[1] && decision[0] && decision[1])
			{
				if (okWaitCntMax + okAlphaCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okEnd[i] = okBegin[i];
					okBegin[i] = false;
				}
				if (okWaitCntMax <= okMoveCnt[i])
				{
					okAlpha[i] -= okAlphaSpeedY;
					//okAlpha[i] = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okWaitCntMax), 0.0f, 1.0f);
				}

				okMoveCnt[i]++;
			}
			else if (okAlphaFlg[i] && !decision[i])
			{
				if (okWaitCntMax + okAlphaCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okBegin[i] = false;
					okEnd[i] = false;

					//okPos[0] = DirectX::XMFLOAT2(okLeftPosX, okPosY);
					//okAlpha[0] = 1.0f;

					okAlphaFlg[i] = false;
				}
				if (okWaitCntMax <= okMoveCnt[i])
				{
					okAlpha[i] -= okAlphaSpeedY;
					//okAlpha[i] = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okWaitCntMax), 0.0f, 1.0f);
				}

				okMoveCnt[i]++;
			}
			else if (okBegin[i] && !okAlphaFlg[i])
			{
				if (okMoveCntMax <= okMoveCnt[i])
				{
					okMoveCnt[i] = 0;
					okAlphaFlg[i] = true;
					break;
				}
				//okPos[i].y -= okMoveSpeedY;
				okPos[i].y = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okMoveCntMax), okGoalPosY, okPosY);

				okMoveCnt[i]++;
			}
		}
		break;

	default: break;
	}
}

void LevelSelector::Operation()
{
	int index = decision[0] ? 1 : 0;

	switch (gameMode)
	{
	case SelectGameMode::Single:
		if (pad[0].bAt)
		{
			okBegin[0] = true;
			okPos[0].y = okPosY;
			okAlpha[0] = 1.0f;

			decision[0] = true;
		}
		if (pad[0].bBt)
		{
			okAlpha[0] = true;
			decision[0] = false;
		}
		if (pad[0].bLEFTt)
		{
			if (levelMin < levelNum[0])
			{
				levelNum[0]--;
			}
		}
		if (pad[0].bRIGHTt)
		{
			if (levelNum[0] < levelMax)
			{
				levelNum[0]++;
			}
		}
		break;
	case SelectGameMode::CPU:
		if (decision[0] && decision[1])
		{
			if (pad[0].bAt)
			{
				cpuPoworDecision = true;
			}
			if (pad[0].bBt)
			{
				decision[1] = false;
			}
			if (pad[0].bLEFTt)
			{
				if (cpuPoworMin < cpuPoworNum)
				{
					cpuPoworNum--;
				}
			}
			if (pad[0].bRIGHTt)
			{
				if (cpuPoworNum < cpuPoworMax)
				{
					cpuPoworNum++;
				}
			}
		}
		else
		{
			if (pad[0].bAt)
			{
				okBegin[index] = true;
				okPos[index].y = okPosY;
				okAlpha[index] = 1.0f;

				decision[index] = true;
			}
			if (pad[0].bBt)
			{
				if (0 < index)
				{
					okAlpha[index - 1] = true;
					decision[index - 1] = false;
				}
			}
			if (pad[0].bLEFTt)
			{
				if (levelMin < levelNum[index])
				{
					levelNum[index]--;
				}
			}
			if (pad[0].bRIGHTt)
			{
				if (levelNum[index] < levelMax)
				{
					levelNum[index]++;
				}
			}
		}
		break;
	case SelectGameMode::Multi:
		for (int i = 0; i < 2; i++)
		{
			if (pad[i].bAt)
			{
				okBegin[i] = true;
				okPos[i].y = okPosY;
				okAlpha[i] = 1.0f;

				decision[i] = true;
			}
			if (pad[i].bBt)
			{
				if (0 < i)
				{
					okAlpha[i - 1] = true;
					decision[i - 1] = false;
				}

				decision[i] = false;
			}
			if (pad[i].bLEFTt)
			{
				if (levelMin < levelNum[i])
				{
					levelNum[i]--;
				}
			}
			if (pad[i].bRIGHTt)
			{
				if (levelNum[i] < levelMax)
				{
					levelNum[i]++;
				}
			}
		}
		break;
	default: break;
	}
}