#include "PlayStyleSelector.h"
#include "SceneManager.h"
#include "Production.h"
#include "Sound.h"

#include "input_device.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void PlayStyleSelector::Construct()
{
	sprSelect = RESOURCE->GetSpriteData(Resource::Texture::Select);
	sprSelectFrameL = RESOURCE->GetSpriteData(Resource::Texture::SelectFrameL);
	sprSelectFrameM = RESOURCE->GetSpriteData(Resource::Texture::SelectFrameM);
	sprSelectFrameS = RESOURCE->GetSpriteData(Resource::Texture::SelectFrameS);
	sprSelectArrow = RESOURCE->GetSpriteData(Resource::Texture::SelectArrow);

	frameLPos = DirectX::XMFLOAT2(154.0f, 295.0f);
	arrowPos[0] = DirectX::XMFLOAT2(77.0f, 517.0f);
	arrowPos[1] = DirectX::XMFLOAT2(780.0f, 517.0f);
	frameMPos[0] = DirectX::XMFLOAT2(154.0f, 295.0f);
	frameMPos[4] = DirectX::XMFLOAT2(-424.0f, 415.0f);
	frameMSize[0] = DirectX::XMFLOAT2(600.0f, 600.0f);
	frameMSize[4] = DirectX::XMFLOAT2(480.0f, 480.0f);
	for (int i = 1; i < 4; i++)
	{
		frameMPos[i] = DirectX::XMFLOAT2(851.0f + 519.0f * (i - 1), 415.0f);
		frameMSize[i] = DirectX::XMFLOAT2(480.0f, 480.0f);
	}
	for (int i = 0; i < 5; i++)
	{
		frameSPos[i] = DirectX::XMFLOAT2(774.0f + 78.0f * i, 164.0f);
	}

	styleNum = styleNumDefault;
}

void PlayStyleSelector::Init(ISelector* selector)
{
	gameMode = SelectGameMode::Single;
	selectMode = SelectMode::PlayStyle;
	thisSelectMode = SelectMode::PlayStyle;
	updateState = UpdateState::BeginMove;
	if (selector == nullptr)
	{
		// 初めは、このセレクターから始まるので前が存在しない。
		// それによりこのセレクターを指定しておく。
		previousSelectorMode = SelectMode::PlayStyle;
	}
	else
	{
		previousSelectorMode = selector->GetThisSelectMode();
	}

	selectMoveRight = false;
	selectMoveLeft = false;
	moveCnt = 0;

	endMoveCnt = 0;
}

bool PlayStyleSelector::Update()
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

void PlayStyleSelector::BeginMove()
{
	if (previousSelectorMode == SelectMode::PlayStyle)
	{
		updateState = UpdateState::Choice;
	}
	else
	{
		/*frameLPos.y -= endMoveSpeedY;
		arrowPos[0].y -= endMoveSpeedY;
		arrowPos[1].y -= endMoveSpeedY;
		for (int i = 0; i < styleNumMax + 1; i++)
		{
			frameMPos[i].y -= endMoveSpeedY;
			frameSPos[i].y += frameSEndMoveSpeedY;
		}*/

		frameLPos.y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 295.0f, 1080.0f);
		arrowPos[0].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 517.0f, 1417.0f);
		arrowPos[1].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 517.0f, 1417.0f);
		frameMPos[styleNum].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 295.0f, 1195.0f);
		for (int i = 0; i < styleNumMax + 1; i++)
		{
			if (i != styleNum) frameMPos[i].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 415.0f, 1315.0f);
			frameSPos[i].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 164.0f, -60.0f);
		}

		if (endMoveCntMax <= endMoveCnt++)
		{
			endMoveCnt = 0;
			updateState = UpdateState::Choice;
		}
	}
}

bool PlayStyleSelector::EndMove()
{
	/*frameLPos.y += endMoveSpeedY;
	arrowPos[0].y += endMoveSpeedY;
	arrowPos[1].y += endMoveSpeedY;
	for (int i = 0; i < styleNumMax + 1; i++)
	{
		frameMPos[i].y += endMoveSpeedY;
		frameSPos[i].y -= frameSEndMoveSpeedY;
	}*/

	frameLPos.y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 1080.0f, 295.0f);
	arrowPos[0].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 1417.0f, 517.0f);
	arrowPos[1].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 1417.0f, 517.0f);
	for (int i = 0; i < styleNumMax + 1; i++)
	{
		frameMPos[i].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), 1315.0f, 415.0f);
		frameSPos[i].y = easing::OutExp(static_cast<float>(endMoveCnt), static_cast<float>(endMoveCntMax), -60.0f, 164.0f);
	}

	if (endMoveCntMax <= endMoveCnt++)
	{
		return true;
	}

	return false;
}

void PlayStyleSelector::Choice()
{
	if ((pad[0].bAt || GetAsyncKeyState('N') & 1) && !selectMoveLeft && !selectMoveRight)
	{
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
		updateState = UpdateState::EndMove;
		switch (static_cast<SelectGameMode>(styleNum))
		{
		case SelectGameMode::Single:
			gameMode = SelectGameMode::Single;
			selectMode = SelectMode::CharacterSelect;
			break;

		case SelectGameMode::Multi:
			//if (2 == PAD.lpJoystick.size())
			{
				gameMode = SelectGameMode::Multi;
				selectMode = SelectMode::CharacterSelect;
			}
			break;
		
		case SelectGameMode::CPU:
			gameMode = SelectGameMode::CPU;
			selectMode = SelectMode::CharacterSelect;
			break;


		default: break;
		}
	}

	Operation();
}

void PlayStyleSelector::Operation()
{
	if (selectMoveLeft)
	{
		int backNum = styleNum + 1;
		if (backNum == styleNumMax + 1)
		{
			backNum = styleNumMin;
		}

		int rightFrameNum = styleNum - 1;
		if (rightFrameNum == styleNumMin - 1)
		{
			rightFrameNum = styleNumMax;
		}

		if (1920.0f <= frameMPos[rightFrameNum].x)
		{
			frameMPos[rightFrameNum].x = -424.0f - (frameMMoveX * (moveCntMax - moveCnt));
		}

		frameMPos[styleNum].x += frameLToMMoveX;
		frameMPos[styleNum].y -= frameLToMMoveY;
		frameMPos[backNum].x += frameMToLMoveX;
		frameMPos[backNum].y += frameMToLMoveY;

		frameMSize[styleNum].x += frameMScale;
		frameMSize[styleNum].y += frameMScale;
		frameMSize[backNum].x -= frameMScale;
		frameMSize[backNum].y -= frameMScale;

		for (int i = 0; i < styleNumMax + 1; i++)
		{
			if (i == styleNum || i == backNum) continue;
			frameMPos[i].x += frameMMoveX;
		}

		moveCnt++;
		if (moveCntMax <= moveCnt)
		{
			moveCnt = 0;
			selectMoveLeft = false;
		}
	}
	else if (selectMoveRight)
	{
		int backNum = styleNum - 1;
		if (backNum == styleNumMin - 1)
		{
			backNum = styleNumMax;
		}

		int leftFrameNum = backNum - 1;
		if (leftFrameNum == styleNumMin - 1)
		{
			leftFrameNum = styleNumMax;
		}

		if (frameMPos[leftFrameNum].x + 480.0f <= 0.0f)
		{
			int leftFrameBackNum = leftFrameNum - 1;
			if (leftFrameBackNum == styleNumMin - 1)
			{
				leftFrameBackNum = styleNumMax;
			}

			frameMPos[leftFrameNum].x = frameMPos[leftFrameBackNum].x + 519.0f;
		}

		frameMPos[styleNum].x -= frameMToLMoveX;
		frameMPos[styleNum].y -= frameMToLMoveY;
		frameMPos[backNum].x -= frameLToMMoveX;
		frameMPos[backNum].y += frameLToMMoveY;

		frameMSize[styleNum].x += frameMScale;
		frameMSize[styleNum].y += frameMScale;
		frameMSize[backNum].x -= frameMScale;
		frameMSize[backNum].y -= frameMScale;

		for (int i = 0; i < styleNumMax + 1; i++)
		{
			if (i == styleNum || i == backNum) continue;
			frameMPos[i].x -= frameMMoveX;
		}

		moveCnt++;
		if (moveCntMax <= moveCnt)
		{
			moveCnt = 0;
			selectMoveRight = false;
		}
	}
	else
	{
		if (pad[0].bLEFTt)
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			styleNum--;
			selectMoveLeft = true;
			moveCnt = 0;
			if (styleNum < styleNumMin)
			{
				styleNum = styleNumMax;
			}
		}
		if (pad[0].bRIGHTt)
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			styleNum++;
			selectMoveRight = true;
			moveCnt = 0;
			if (styleNumMax < styleNum)
			{
				styleNum = styleNumMin;
			}
		}
	}
}

void PlayStyleSelector::Draw()
{
	sprSelectFrameM->Begin();
	for (int i = 0; i < styleNumMax + 1; i++)
	{
		if (i == styleNum && (!selectMoveRight && !selectMoveLeft)) continue;
		sprSelectFrameM->Draw(frameMPos[i].x, frameMPos[i].y, frameMSize[i].x, frameMSize[i].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, i);
	}
	sprSelectFrameM->End();

	if (!selectMoveLeft && !selectMoveRight)
	{
		sprSelectFrameL->Begin();
		sprSelectFrameL->Draw(frameLPos.x, frameLPos.y, 600.0f, 600.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, styleNum);
		sprSelectFrameL->End();
	}

	sprSelectFrameS->Begin();
	for (int i = 0; i < styleNumMax + 1; i++)
	{
		sprSelectFrameS->Draw(frameSPos[i].x, frameSPos[i].y, 60.0f, 60.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, styleNum == i ? 1 : 0);
	}
	sprSelectFrameS->End();

	sprSelectArrow->Begin();
	sprSelectArrow->Draw(arrowPos[0].x, arrowPos[0].y, 54.0f, 138.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0);
	sprSelectArrow->Draw(arrowPos[1].x, arrowPos[1].y, 54.0f, 138.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
	sprSelectArrow->End();
}