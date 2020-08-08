#include "CharacterSelector.h"
#include "PlayStyleSelector.h"
#include "LevelSelector.h"
#include "Resource.h"
#include "input_device.h"


void CharacterSelector::Construct()
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

void CharacterSelector::Init(ISelector* selector)
{
	gameMode = static_cast<PlayStyleSelector*>(selector)->GetSelectGameMode();
	selectMode = SelectMode::CharacterSelect;
	thisSelectMode = SelectMode::CharacterSelect;
	updateState = UpdateState::BeginMove;

	selectCharacterNum[0] = player01CharacterDefault;
	selectCharacterNum[1] = player02CharacterDefault;

	if (selector == nullptr)
	{
		// 初めは、このセレクターから始まるので前が存在しない。
		// それによりこのセレクターを指定しておく。
		previousSelectorMode = SelectMode::CharacterSelect;
	}
	else
	{
		previousSelectorMode = selector->GetThisSelectMode();
	}

	if (previousSelectorMode == SelectMode::LevelSelect)
	{

		selectCharacterNum[0] = static_cast<LevelSelector*>(selector)->GetCharacterNum(0);
		selectCharacterNum[1] = static_cast<LevelSelector*>(selector)->GetCharacterNum(1);
	}

	if (previousSelectorMode == SelectMode::PlayStyle)
	{
		characterPos[0] = DirectX::XMFLOAT2(characterLeftPosX, characterPosY);
		characterPos[1] = DirectX::XMFLOAT2(characterRightPosX, characterPosY);
	}
	else if (gameMode == SelectGameMode::CPU)
	{
		characterPos[1] = DirectX::XMFLOAT2(characterRightPosX, characterPosY);
	}
	characterSize[0] = DirectX::XMFLOAT2(characterSizeX, characterSizeY);
	characterSize[1] = DirectX::XMFLOAT2(characterSizeX, characterSizeY);
	//characterFrameOriginPos = DirectX::XMFLOAT2(characterFrameOriginPosX, characterFrameOriginPosY);
	characterFrameOriginPos = DirectX::XMFLOAT2(characterFrameOriginPosX, characterFrameStartOriginPosY);
	characterFrameSize = DirectX::XMFLOAT2(characterFrameSizeX, characterFrameSizeY);
	characterPick[0] = DirectX::XMFLOAT2(characterFrameOriginPosX, characterFrameStartOriginPosY);
	characterPick[1] = DirectX::XMFLOAT2(characterFrameOriginPosX + characterFrameSizeX, characterFrameStartOriginPosY);

	characterDecision[0] = false;
	characterDecision[1] = false;
	selectCPUCharacterState = 0;

	endMoveReturn = false;
	operationLock = false;

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

	cpuCharacterMove = false;
	cpuCharacterMoveReturn = false;
	cpuCharacterMoveCnt = 0;
}

bool CharacterSelector::Update()
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

void CharacterSelector::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(240.0f, 72.0f, 1440.0f, 160.0f, 0.0f, 160.0f, 1440.0f, 160.0f, 0.0f, 0.0f, 0.0f);
	sprSelect->End();

	sprSelectCPU->Begin();
	for (int i = 0; i < characterMax + 1; i++)
	{
		float x = i < 4 ? characterFrameOriginPos.x + characterFrameSize.x * i : characterFrameOriginPos.x + characterFrameSize.x * (i - 4);
		float y = i < 4 ? characterFrameOriginPos.y : characterFrameOriginPos.y + characterFrameSize.y;
		float texX = characterFrameTexPosX + characterFrameSize.x * i;
		sprSelectCPU->Draw(x, y, characterFrameSize.x, characterFrameSize.y, texX, characterFrameTexPosY, characterFrameSize.x, characterFrameSize.y, 0.0f, 0.0f, 0.0f);
	}
	sprSelectCPU->End();
	
	float texX = 0.0f;
	float texY = 0.0f;
	switch (gameMode)
	{
	case SelectGameMode::Single:
		if (updateState == UpdateState::Choice)
		{
			characterPick[0].x = selectCharacterNum[0] < 4 ? characterFrameOriginPosX + characterFrameSizeX * selectCharacterNum[0] : characterFrameOriginPosX + characterFrameSizeX * (selectCharacterNum[0] - 4);
			characterPick[0].y = selectCharacterNum[0] < 4 ? characterFrameOriginPosY : characterFrameOriginPosY + characterFrameSizeY;
		}
		texX = !characterDecision[0] ? characterPickTexPosX : characterPickTexPosX + characterPickSizeX;
		texY = characterPickTexPosY;
		sprSelectCPU->Begin();
		sprSelectCPU->Draw(characterPick[0].x, characterPick[0].y, characterPickSizeX, characterPickSizeY, texX, texY, characterPickSizeX, characterPickSizeY, 0.0f, 0.0f, 0.0f);
		if (updateState == UpdateState::Choice) sprSelectCPU->Draw(okPos[0].x, okPos[0].y, okSizeX, okSizeY, okTexPosX, okTexPosY, okSizeX, okSizeY, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, okAlpha[0]);
		sprSelectCPU->End();

		sprCharacters[selectCharacterNum[0]]->Begin();
		sprCharacters[selectCharacterNum[0]]->Draw(characterPos[0].x, characterPos[0].y, characterSize[0].x, characterSize[0].y, 0.0f, 0.0f, characterSize[0].x, characterSize[0].y, 0.0f, 0.0f, 0.0f);
		sprCharacters[selectCharacterNum[0]]->End();
		break;
	case SelectGameMode::CPU:
		for (int i = 0; i < 2; i++)
		{
			if (!characterDecision[i - 1] && 1 <= i) continue;
	
			if (updateState == UpdateState::Choice)
			{
				characterPick[i].x = selectCharacterNum[i] < 4 ? characterFrameOriginPosX + characterFrameSizeX * selectCharacterNum[i] : characterFrameOriginPosX + characterFrameSizeX * (selectCharacterNum[i] - 4);
				characterPick[i].y = selectCharacterNum[i] < 4 ? characterFrameOriginPosY : characterFrameOriginPosY + characterFrameSizeY;
			}
			float texX = !characterDecision[i] ? characterPickTexPosX : characterPickTexPosX + characterPickSizeX;
			float texY = characterPickTexPosY + characterPickSizeY * i;
			sprSelectCPU->Begin();
			sprSelectCPU->Draw(characterPick[i].x, characterPick[i].y, characterPickSizeX, characterPickSizeY, texX, texY, characterPickSizeX, characterPickSizeY, 0.0f, 0.0f, 0.0f);
			if (updateState == UpdateState::Choice)
			{
				if (i == 0 || (i == 1 && !cpuCharacterMove))
				{
					sprSelectCPU->Draw(okPos[i].x, okPos[i].y, okSizeX, okSizeY, okTexPosX + okSizeX * i, okTexPosY, okSizeX, okSizeY, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, okAlpha[i]);
				}
			}
			sprSelectCPU->End();

			sprCharacters[selectCharacterNum[i]]->Begin();
			sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
			sprCharacters[selectCharacterNum[i]]->End();
		}
		break;
	case SelectGameMode::Multi:
		for (int i = 0; i < 2; i++)
		{
			if (updateState == UpdateState::Choice)
			{
				characterPick[i].x = selectCharacterNum[i] < 4 ? characterFrameOriginPosX + characterFrameSizeX * selectCharacterNum[i] : characterFrameOriginPosX + characterFrameSizeX * (selectCharacterNum[i] - 4);
				characterPick[i].y = selectCharacterNum[i] < 4 ? characterFrameOriginPosY : characterFrameOriginPosY + characterFrameSizeY;
			}
			float texX = !characterDecision[i] ? characterPickTexPosX : characterPickTexPosX + characterPickSizeX;
			float texY = characterPickTexPosY + characterPickSizeY * i;
			sprSelect->Begin();
			sprSelect->Draw(characterPick[i].x, characterPick[i].y, characterPickSizeX, characterPickSizeY, texX, texY, characterPickSizeX, characterPickSizeY, 0.0f, 0.0f, 0.0f);
			if (updateState == UpdateState::Choice) sprSelect->Draw(okPos[i].x, okPos[i].y, okSizeX, okSizeY, okTexPosX + okSizeX * i, okTexPosY, okSizeX, okSizeY, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, okAlpha[i]);
			sprSelect->End();

			sprCharacters[selectCharacterNum[i]]->Begin();
			sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
			sprCharacters[selectCharacterNum[i]]->End();
		}
		break;
	default: break;
	}
}

void CharacterSelector::BeginMove()
{
	if (moveCntMax <= moveCnt++)
	{
		moveCnt = 0;
		updateState = UpdateState::Choice;
		return;
	}
	/*if (previousSelectorMode == SelectMode::PlayStyle)
	{
		characterPos[0].x += moveSpeedX;
		if (gameMode != SelectGameMode::CPU) characterPos[1].x -= moveSpeedX;
	}
	characterFrameOriginPos.y -= moveSpeedY;
	characterPick[0].y -= moveSpeedY;
	characterPick[1].y -= moveSpeedY;*/

	if (previousSelectorMode == SelectMode::PlayStyle)
	{
		characterPos[0].x = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 246.0f, -416.0f);
		if (gameMode != SelectGameMode::CPU)
		{
			characterPos[1].x = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 1674.0f, 2336.0f);
		}
	}
	characterFrameOriginPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 508.0f, characterFrameStartOriginPosY);
	characterPick[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 508.0f, characterFrameStartOriginPosY);
	characterPick[1].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 508.0f, characterFrameStartOriginPosY);
}

bool CharacterSelector::EndMove()
{
	/*if (endMoveReturn)
	{
		characterPos[0].x -= moveSpeedX;
	}
	characterFrameOriginPos.y += moveSpeedY;
	characterPick[0].y += moveSpeedY;
	characterPick[1].y += moveSpeedY;*/

	if (endMoveReturn)
	{
		characterPos[0].x = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), -416.0f, 246.0f);
	}
	characterFrameOriginPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);
	characterPick[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);
	characterPick[1].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);

	if (moveCntMax <= moveCnt++)
	{
		moveCnt = 0;
		return true;
	}

	return false;
}

void CharacterSelector::Choice()
{
	if (pad[0].bBt && !characterDecision[0] && !characterDecision[1] && !okEnd[0] && !okEnd[1])
	{
		updateState = UpdateState::EndMove;
		selectMode = SelectMode::PlayStyle;
		endMoveReturn = true;
	}
	else
	{
		if (gameMode == SelectGameMode::Single && characterDecision[0] && okEnd[0])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::LevelSelect;
		}
		else if (gameMode == SelectGameMode::Multi && characterDecision[0] && characterDecision[1] && okEnd[0] && okEnd[1])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::LevelSelect;
		}
		else if (gameMode == SelectGameMode::CPU && selectMode == SelectMode::CharacterSelect && characterDecision[0] && characterDecision[1] && okEnd[0] && okEnd[1])
		{
			updateState = UpdateState::EndMove;
			selectMode = SelectMode::CPULevelSelect;
		}
	}

	Operation();
	CPUMove();
	OkMove();
}

void CharacterSelector::CPUMove()
{
	if (cpuCharacterMove)
	{
		if (moveCntMax <= cpuCharacterMoveCnt++)
		{
			cpuCharacterMoveCnt = 0;
			cpuCharacterMove = false;
			if (cpuCharacterMoveReturn)
			{
				characterDecision[selectCPUCharacterState] = false;
				cpuCharacterMoveReturn = false;
			}
			return;
		}
		if (cpuCharacterMoveReturn)
		{
			//characterPos[1].x += moveSpeedX;
			characterPos[1].x = easing::OutExp(static_cast<float>(cpuCharacterMoveCnt), static_cast<float>(moveCntMax), characterRightPosX, 1674.0f);
		}
		else
		{
			//characterPos[1].x -= moveSpeedX;
			characterPos[1].x = easing::OutExp(static_cast<float>(cpuCharacterMoveCnt), static_cast<float>(moveCntMax), 1674.0f, characterRightPosX);
		}
	}
}

void CharacterSelector::OkMove()
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
	case SelectGameMode::Multi:
		for (int i = 0; i < 2; i++)
		{
			if (okAlphaFlg[0] && okAlphaFlg[1] && characterDecision[0] && characterDecision[1])
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
					//okAlpha[i] = easing::OutExp(static_cast<float>(okMoveCnt[i]), static_cast<float>(okMoveCntMax), 0.0f, 1.0f);
				}

				okMoveCnt[i]++;
			}
			else if (okAlphaFlg[i] && !characterDecision[i])
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

	default: break;
	}
}

void CharacterSelector::Operation()
{
	if (cpuCharacterMove || operationLock) return;

	switch (gameMode)
	{
	case SelectGameMode::Single:
		if (pad[0].bAt)
		{
			okBegin[0] = true;
			okPos[0].y = okPosY;
			okAlpha[0] = 1.0f;

			characterDecision[0] = true;
		}
		if (pad[0].bBt)
		{
			okAlpha[0] = true;
			characterDecision[0] = false;
		}
		if (pad[0].bLEFTt)
		{
			if (0 < selectCharacterNum[0])
			{
				selectCharacterNum[0]--;
			}
		}
		if (pad[0].bRIGHTt)
		{
			if (selectCharacterNum[0] < characterMax)
			{
				selectCharacterNum[0]++;
			}
		}
		if (pad[0].bUPt)
		{
			if ((characterMax + 1) / 2 <= selectCharacterNum[0])
			{
				selectCharacterNum[0] -= (characterMax + 1) / 2;
			}
		}
		if (pad[0].bDOWNt)
		{
			if (selectCharacterNum[0] < (characterMax + 1) / 2)
			{
				selectCharacterNum[0] += (characterMax + 1) / 2;
			}
		}
		break;
	case SelectGameMode::CPU:
		if (pad[0].bAt && !okAlphaFlg[selectCPUCharacterState])
		{
			if (selectCPUCharacterState == 0) cpuCharacterMove = true;
			if (selectCPUCharacterState == 1) operationLock = true;

			okBegin[selectCPUCharacterState] = true;
			okPos[selectCPUCharacterState].y = okPosY;
			okAlpha[selectCPUCharacterState] = 1.0f;

			characterDecision[selectCPUCharacterState] = true;
			//selectCharacterNum[selectCPUCharacterState + 1] = selectCharacterNum[selectCPUCharacterState];
			selectCPUCharacterState++;
		}
		if (pad[0].bBt && 0 < selectCPUCharacterState)
		{
			if (selectCPUCharacterState == 1)
			{
				cpuCharacterMove = true;
				cpuCharacterMoveReturn = true;
			}

			characterDecision[selectCPUCharacterState] = false;
			selectCPUCharacterState--;
			okAlpha[selectCPUCharacterState] = true;
		}
		if (pad[0].bLEFTt)
		{
			if (0 < selectCharacterNum[selectCPUCharacterState])
			{
				selectCharacterNum[selectCPUCharacterState]--;
			}
		}
		if (pad[0].bRIGHTt)
		{
			if (selectCharacterNum[selectCPUCharacterState] < characterMax)
			{
				selectCharacterNum[selectCPUCharacterState]++;
			}
		}
		if (pad[0].bUPt)
		{
			if ((characterMax + 1) / 2 <= selectCharacterNum[selectCPUCharacterState])
			{
				selectCharacterNum[selectCPUCharacterState] -= (characterMax + 1) / 2;
			}
		}
		if (pad[0].bDOWNt)
		{
			if (selectCharacterNum[selectCPUCharacterState] < (characterMax + 1) / 2)
			{
				selectCharacterNum[selectCPUCharacterState] += (characterMax + 1) / 2;
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

				characterDecision[i] = true;
			}
			if (pad[i].bBt)
			{
				okAlpha[i] = true;
				characterDecision[i] = false;
			}
			if (pad[i].bLEFTt)
			{
				if (0 < selectCharacterNum[i])
				{
					selectCharacterNum[i]--;
				}
			}
			if (pad[i].bRIGHTt)
			{
				if (selectCharacterNum[i] < characterMax)
				{
					selectCharacterNum[i]++;
				}
			}
			if (pad[i].bUPt)
			{
				if ((characterMax + 1) / 2 <= selectCharacterNum[i])
				{
					selectCharacterNum[i] -= (characterMax + 1) / 2;
				}
			}
			if (pad[i].bDOWNt)
			{
				if (selectCharacterNum[i] < (characterMax + 1) / 2)
				{
					selectCharacterNum[i] += (characterMax + 1) / 2;
				}
			}
		}
		break;
	default: break;
	}
}