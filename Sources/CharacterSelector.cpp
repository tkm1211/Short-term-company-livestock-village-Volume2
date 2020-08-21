#include "CharacterSelector.h"
#include "PlayStyleSelector.h"
#include "LevelSelector.h"
#include "Resource.h"
#include "Sound.h"
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
	//characterPick[0] = DirectX::XMFLOAT2(characterFrameOriginPosX, characterFrameStartOriginPosY);
	//characterPick[1] = DirectX::XMFLOAT2(characterFrameOriginPosX + characterFrameSizeX, characterFrameStartOriginPosY);
	for (int i = 0; i < 2; i++)
	{
		characterPick[i].x = selectCharacterNum[i] < 4 ? characterFrameOriginPosX + characterFrameSizeX * selectCharacterNum[i] : characterFrameOriginPosX + characterFrameSizeX * (selectCharacterNum[i] - 4);
		characterPick[i].y = selectCharacterNum[i] < 4 ? characterFrameStartOriginPosY : characterFrameStartOriginPosY + characterFrameSizeY;
	}

	stringPos = DirectX::XMFLOAT2(stringStartPosX, stringStartPosY);
	stringMoveY = 0.0f;
	stringMoveSpeed = 0.0f;

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

	accelerationCount[0] = DirectX::XMINT2(0, 0);
	accelerationCount[1] = DirectX::XMINT2(0, 0);

	animCnt = 0;
	animFrame = 0;
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

	if (animCnt++ % 5 == 0 && animCnt != 0)
	{
		animFrame++;
		if (CHARACTER_ANIMATION_MAX <= animFrame)
		{
			animFrame = 0;
		}
	}

	return ch;
}

void CharacterSelector::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(stringPos.x, stringPos.y + stringMoveY, 1440.0f, 160.0f, 0.0f, 160.0f, 1440.0f, 160.0f, 0.0f, 0.0f, 0.0f);
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
		if (selectCharacterNum[0] != 7) sprCharacters[selectCharacterNum[0]]->Draw(characterPos[0].x, characterPos[0].y, characterSize[0].x, characterSize[0].y, characterSize[0].x * animFrame, 0.0f, characterSize[0].x, characterSize[0].y, 0.0f, 0.0f, 0.0f);
		else sprCharacters[selectCharacterNum[0]]->Draw(characterPos[0].x, characterPos[0].y, characterSize[0].x, characterSize[0].y, 0.0f, 0.0f, characterSize[0].x, characterSize[0].y, 0.0f, 0.0f, 0.0f);
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
			if (selectCharacterNum[i] != 7) sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, characterSize[0].x * animFrame, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
			else sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
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
			if (selectCharacterNum[i] != 7) sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, characterSize[0].x * animFrame, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
			else sprCharacters[selectCharacterNum[i]]->Draw(characterPos[i].x, characterPos[i].y, i == 0 ? characterSize[i].x : -characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, characterSize[i].x, characterSize[i].y, 0.0f, 0.0f, 0.0f);
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
	stringPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), stringEndPosY, stringStartPosY);
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
		if (gameMode != SelectGameMode::CPU)
		{
			characterPos[1].x = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), 2336.0f, 1674.0f);
		}
	}
	characterFrameOriginPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);
	characterPick[0].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);
	characterPick[1].y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), characterFrameStartOriginPosY, 508.0f);
	stringPos.y = easing::OutExp(static_cast<float>(moveCnt), static_cast<float>(moveCntMax), stringStartPosY, stringEndPosY);

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
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::BACK].get());
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

	stringMoveY = stringMaxMove * sinf(stringMoveSpeed);
	stringMoveSpeed += stringSpeed;
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
			operationLock = true;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
		}
		if (pad[0].bBt)
		{
			okAlpha[0] = true;
			characterDecision[0] = false;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::BACK].get());
		}
		if (pad[0].bLEFTs || pad[0].sLX < 0)
		{
			if (0 < selectCharacterNum[0])
			{
				if (++accelerationCount[0].x == 1)
				{
					selectCharacterNum[0]--;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
				if (accelerationCount[0].x >= ACCEL_START)
				{
					if (accelerationCount[0].x % ACCEL_MOVE_PER == 0)
					{
						selectCharacterNum[0]--;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
		{
			accelerationCount[0].x = 0;
		}
		if (pad[0].bRIGHTs || 0 < pad[0].sLX)
		{
			if (selectCharacterNum[0] < characterMax)
			{
				if (++accelerationCount[0].y == 1)
				{
					selectCharacterNum[0]++;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
				if (accelerationCount[0].y >= ACCEL_START)
				{
					if (accelerationCount[0].y % ACCEL_MOVE_PER == 0)
					{
						selectCharacterNum[0]++;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
		{
			accelerationCount[0].y = 0;
		}
		if (pad[0].bUPt || pad[0].sLY < 0)
		{
			if ((characterMax + 1) / 2 <= selectCharacterNum[0])
			{
				selectCharacterNum[0] -= (characterMax + 1) / 2;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
		if (pad[0].bDOWNt || 0 < pad[0].sLY)
		{
			if (selectCharacterNum[0] < (characterMax + 1) / 2)
			{
				selectCharacterNum[0] += (characterMax + 1) / 2;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
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
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
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
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::BACK].get());
		}
		if (pad[0].bLEFTs || pad[0].sLX < 0)
		{
			if (0 < selectCharacterNum[selectCPUCharacterState])
			{
				if (++accelerationCount[selectCPUCharacterState].x == 1)
				{
					selectCharacterNum[selectCPUCharacterState]--;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
				if (accelerationCount[selectCPUCharacterState].x >= ACCEL_START)
				{
					if (accelerationCount[selectCPUCharacterState].x % ACCEL_MOVE_PER == 0)
					{
						selectCharacterNum[selectCPUCharacterState]--;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
		{
			accelerationCount[selectCPUCharacterState].x = 0;
		}
		if (pad[0].bRIGHTs || 0 < pad[0].sLX)
		{
			if (selectCharacterNum[selectCPUCharacterState] < characterMax)
			{
				if (++accelerationCount[selectCPUCharacterState].y == 1)
				{
					selectCharacterNum[selectCPUCharacterState]++;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
				if (accelerationCount[selectCPUCharacterState].y >= ACCEL_START)
				{
					if (accelerationCount[selectCPUCharacterState].y % ACCEL_MOVE_PER == 0)
					{
						selectCharacterNum[selectCPUCharacterState]++;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
		{
			accelerationCount[selectCPUCharacterState].y = 0;
		}
		if (pad[0].bUPt || pad[0].sLY < 0)
		{
			if ((characterMax + 1) / 2 <= selectCharacterNum[selectCPUCharacterState])
			{
				selectCharacterNum[selectCPUCharacterState] -= (characterMax + 1) / 2;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
		if (pad[0].bDOWNt || 0 < pad[0].sLY)
		{
			if (selectCharacterNum[selectCPUCharacterState] < (characterMax + 1) / 2)
			{
				selectCharacterNum[selectCPUCharacterState] += (characterMax + 1) / 2;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
		break;
	case SelectGameMode::Multi:
		for (int i = 0; i < 2; i++)
		{
			if (pad[i].bAt && !characterDecision[i])
			{
				okBegin[i] = true;
				okPos[i].y = okPosY;
				okAlpha[i] = 1.0f;

				characterDecision[i] = true;
				if (characterDecision[0] && characterDecision[1]) operationLock = true;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
			}
			if (pad[i].bBt && characterDecision[i])
			{
				okAlpha[i] = true;
				characterDecision[i] = false;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::BACK].get());
			}
			
			if (characterDecision[i]) continue;

			if (pad[i].bLEFTs || pad[i].sLX < 0)
			{
				if (0 < selectCharacterNum[i])
				{
					if (++accelerationCount[i].x == 1)
					{
						selectCharacterNum[i]--;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
					if (accelerationCount[i].x >= ACCEL_START)
					{
						if (accelerationCount[i].x % ACCEL_MOVE_PER == 0)
						{
							selectCharacterNum[i]--;
							pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
						}
					}
				}
			}
			else
			{
				accelerationCount[i].x = 0;
			}
			if (pad[i].bRIGHTs || 0 < pad[i].sLX)
			{
				if (selectCharacterNum[i] < characterMax)
				{
					if (++accelerationCount[i].y == 1)
					{
						selectCharacterNum[i]++;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
					if (accelerationCount[i].y >= ACCEL_START)
					{
						if (accelerationCount[i].y % ACCEL_MOVE_PER == 0)
						{
							selectCharacterNum[i]++;
							pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
						}
					}
				}
			}
			else
			{
				accelerationCount[i].y = 0;
			}
			if (pad[i].bUPt || pad[i].sLY < 0)
			{
				if ((characterMax + 1) / 2 <= selectCharacterNum[i])
				{
					selectCharacterNum[i] -= (characterMax + 1) / 2;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
			}
			if (pad[i].bDOWNt || 0 < pad[i].sLY)
			{
				if (selectCharacterNum[i] < (characterMax + 1) / 2)
				{
					selectCharacterNum[i] += (characterMax + 1) / 2;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
			}
		}
		break;
	default: break;
	}
}