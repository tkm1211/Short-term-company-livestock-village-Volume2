#include "SceneSelect.h"
#include "SceneManager.h"
#include "Production.h"
#include "easing.h"
#include "Player.h"
#include "Sound.h"

#include <framework.h>
//#include <GamePad.h>
#include <input_device.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#define CNT_MAX 60
#define CNT_MAX_2 120
#define CNT_MAX_3 60
#define CNT_MAX_4 60

#define CNT_MAX_INTERPOLATION 5
#define CNT_MAX_2_INTERPOLATION 80

//#define DEBUG_TWOPLAY_OF_ONE_PAD

// セレクトメイン
void SceneSelect::Init()
{
	selectState = 0;
	lastSelectState = 0;

	sprSelectUI = std::make_unique<SpriteBatch>(L"Data/modeselect.png", 50, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprChar[0] = std::make_unique<SpriteBatch>(L"Data/character01.png", 5, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprChar[1] = std::make_unique<SpriteBatch>(L"Data/character02.png", 5, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprChar[2] = std::make_unique<SpriteBatch>(L"Data/character03.png", 5, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprChar[3] = std::make_unique<SpriteBatch>(L"Data/character04.png", 5, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprBackBG = std::make_unique<SpriteBatch>(L"Data/selectBG.png");

	logo.pos = DirectX::XMFLOAT2(240.0f, 72.0f);
	logo.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	logo.size = DirectX::XMFLOAT2(1440.0f, 160.0f);

	text1.pos = DirectX::XMFLOAT2(0.0f, 436.0f);
	text1.tex = DirectX::XMFLOAT2(0.0f, 480.0f);
	text1.size = DirectX::XMFLOAT2(292.0f, 232.0f);

	char1.pos = DirectX::XMFLOAT2(246.0f, 436.0f);
	char1.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	char1.size = DirectX::XMFLOAT2(320.0f, 416.0f);

	char2.pos = DirectX::XMFLOAT2(1920.0f, 436.0f);
	char2.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	char2.size = DirectX::XMFLOAT2(319.0f, 416.0f);

	characterNum = CharacterType::CHARACTER_1;
	characterNum2 = CharacterType::CHARACTER_2;

	charAnimCnt = 0;
	charAnimFrame = 0;

	ModeSelectInit();

	pAudio->Play(SOUND->bgmHandle[SOUND->SELECT], true);
	pAudio->SetVolume(SOUND->bgmHandle[SOUND->SELECT], 1.0f, 0);
}

void SceneSelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_GAME))
	{
		if (gameMode == 0)
		{
			PRODUCTION->CSOH(SCENE_MANAGER->GAME, true);
		}
		else
		{
			PRODUCTION->CSOH(SCENE_MANAGER->GAME);
		}
	}
	else if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
	{
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	}
	else
	{
		char1.tex.x = 416.0f * charAnimFrame;
		char2.tex.x = 416.0f * charAnimFrame;

		if (charAnimCnt++ % 5 == 0 && charAnimCnt != 0)
		{
			charAnimFrame++;
			if (6 <= charAnimFrame)
			{
				charAnimFrame = 0;
			}
		}

		if (lastSelectState != selectState)
		{
			if (selectState == SelectState::CHAR_SELECT)
			{
				CharSelectInit();
			}
			else if (selectState == SelectState::LEVEL_SELECT)
			{
				levelSelectInit();
			}
		}
		lastSelectState = selectState;

		switch (selectState)
		{
		case SelectState::MODE_SELECT:
			ModeSelectUpdate();
			break;
		case SelectState::CHAR_SELECT:
			CharSelectUpdate();
			break;
		case SelectState::LEVEL_SELECT:
			levelSelectUpdate();
			break;
		}
	}
}

void SceneSelect::Render()
{
	DirectX::XMFLOAT4 nomalColor(1.0f, 1.0f, 1.0f, 1.0f);

	sprBackBG->Begin();
	sprBackBG->Draw(0, 0, 1920, 1080, 0.0f);
	sprBackBG->End();

	switch (selectState)
	{
	case SelectState::MODE_SELECT:
		ModeSelectDraw();
		break;
	case SelectState::CHAR_SELECT:
		CharSelectDraw();
		break;
	case SelectState::LEVEL_SELECT:
		levelSelectDraw();
		break;
	}

	sprSelectUI->Begin();

	sprSelectUI->Draw(logo.pos, logo.size, logo.tex, logo.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);

	sprSelectUI->End();

	sprChar[characterNum]->Begin();
	sprChar[characterNum]->Draw(char1.pos, char1.size, char1.tex, char1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	sprChar[characterNum]->End();

	sprChar[characterNum2]->Begin();
	sprChar[characterNum2]->Draw(DirectX::XMFLOAT2(char2.pos.x + char2.size.x, char2.pos.y), DirectX::XMFLOAT2(-char2.size.x, char2.size.y), char2.tex, char2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	sprChar[characterNum2]->End();


	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_GAME))
	{
		PRODUCTION->Draw();
	}
}

// モードセレクト
void SceneSelect::ModeSelectInit()
{
	gameMode = GameMode::ONE_PLAYER;

	onePlayMode.pos = DirectX::XMFLOAT2(586.0f, 370.0f);
	onePlayMode.tex = DirectX::XMFLOAT2(0.0f, 712.0f);
	onePlayMode.size = DirectX::XMFLOAT2(750.0f, 150.0f);

	twoPlayMode.pos = DirectX::XMFLOAT2(586.0f, 596.0f);
	twoPlayMode.tex = DirectX::XMFLOAT2(750.0f, 862.0f);
	twoPlayMode.size = DirectX::XMFLOAT2(750.0f, 150.0f);

	isOnePlayer = true;
	isTwoPlayer = false;

	isDecision = false;
	isNextMove = false;
	cnt = 0.0f;
	sleep = 0;

	alpha = 1.0f;

	isPlaySound = false;
}

void SceneSelect::ModeSelectUpdate()
{
	if (pad[0].bAt/* || InputTrigger(XINPUT_A, 1)*/)
	{
		if (gameMode == GameMode::ONE_PLAYER)
		{
			if (!isPlaySound)
			{
				pAudio->Play(SOUND->seHandle[SOUND->OK]);
				isPlaySound = true;
			}
			isNextMove = true;
		}
#ifdef DEBUG_TWOPLAY_OF_ONE_PAD
		else if (gameMode == GameMode::TWO_PLAYER/* && 2 == dInput.js.size()*/)
		{
			isDecision = true;
		}
#else
		else if (gameMode == GameMode::TWO_PLAYER && 2 == PAD.lpJoystick.size())
		{
			if (!isPlaySound)
			{
				pAudio->Play(SOUND->seHandle[SOUND->OK]);
				isPlaySound = true;
			}
			isDecision = true;
		}
#endif // DEBUG_TWOPLAY_OF_ONE_PAD
	}

	if (isDecision)
	{
		if (sleep <= 0)
		{
			char2.pos.x = easing::OutExp(cnt, CNT_MAX, 1355.0f, 1920.0f);
			//char2.pos.x -= CHAR2_POS_X / CNT_MAX;
			if (CNT_MAX - CNT_MAX_INTERPOLATION <= cnt++)
			{
				char2.pos.x = 1355.0f;
				cnt = 0;
				sleep++;
			}
		}
		else
		{
			if (10 <= sleep++)
			{
				//isDecision = false;
				isNextMove = true;
				isDecision = false;
			}
		}
	}
	else if (isNextMove)
	{
		logo.pos.y = easing::OutExp(cnt, CNT_MAX_2, -600.0f, 72.0f);
		onePlayMode.pos.y = easing::OutExp(cnt, CNT_MAX_2, -302.0f, 370.0f);
		twoPlayMode.pos.y = easing::OutExp(cnt, CNT_MAX_2, 1268.0f, 596.0f);
		//alpha = easing::InExp(cnt, CNT_MAX_2 - CNT_MAX_2_INTERPOLATION, 0.0f, 1.0f);
		//logo.pos.y -= POS_Y / CNT_MAX_2;
		//onePlayMode.pos.y -= POS_Y / CNT_MAX_2;
		//twoPlayMode.pos.y += POS_Y / CNT_MAX_2;
		if (35 <= cnt)
		{
			alpha -= 1.0f / 5.0f;
		}
		if (CNT_MAX_2 - CNT_MAX_2_INTERPOLATION <= cnt++)
		{
			isNextMove = false;
			logo.pos.y = -600.0f;
			onePlayMode.pos.y = -302.0f;
			twoPlayMode.pos.y = 1268.0f;
			alpha = 0.0f;
			cnt = 0;
			selectState = SelectState::CHAR_SELECT;
		}
	}
	else
	{
		if (pad[0].bDOWNs || 0 < pad[0].sY /* || InputState(XINPUT_DPAD_DOWN, 1) || DPadInputState(STICK_DOWN, 1)*/)
		{
			if (gameMode < GameMode::TWO_PLAYER)
			{
				text1.tex.x = 292.0f;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				gameMode++;
			}
		}
		if (pad[0].bUPs || pad[0].sY < 0/* || InputState(XINPUT_DPAD_UP, 1) || DPadInputState(STICK_UP, 1)*/)
		{
			if (GameMode::ONE_PLAYER < gameMode)
			{
				text1.tex.x = 0.0f;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				gameMode--;
			}
		}

		switch (gameMode)
		{
		case GameMode::ONE_PLAYER:
			isOnePlayer = true;
			isTwoPlayer = false;
			break;
		case GameMode::TWO_PLAYER:
			isOnePlayer = false;
			isTwoPlayer = true;
			break;
		}
	}

	if (isOnePlayer)
	{
		onePlayMode.tex.x = 0.0f;
	}
	else
	{
		onePlayMode.tex.x = 750.0f;
	}
	if (isTwoPlayer)
	{
		twoPlayMode.tex.x = 0.0f;
	}
	else
	{
		twoPlayMode.tex.x = 750.0f;
	}
}

void SceneSelect::ModeSelectDraw()
{
	DirectX::XMFLOAT4 nomalColor(1.0f, 1.0f, 1.0f, 1.0f);

	sprSelectUI->Begin();

	sprSelectUI->Draw(onePlayMode.pos, onePlayMode.size, onePlayMode.tex, onePlayMode.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	sprSelectUI->Draw(twoPlayMode.pos, twoPlayMode.size, twoPlayMode.tex, twoPlayMode.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	sprSelectUI->Draw(text1.pos, text1.size, text1.tex, text1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha)); // TODO : キャラクター吹き出しをどうするか？

	sprSelectUI->End();
}

// キャラクターセレクト
void SceneSelect::CharSelectInit()
{
	//logo.pos = DirectX::XMFLOAT2(240.0f, 72.0f);
	logo.tex = DirectX::XMFLOAT2(0.0f, 160.0f);

	charList.pos = DirectX::XMFLOAT2(660.0f, logo.pos.y + 436.0f);
	charList.tex = DirectX::XMFLOAT2(0.0f, 1012.0f);
	charList.size = DirectX::XMFLOAT2(600.0f, 150.0f);

	cursol1.pos = DirectX::XMFLOAT2(660.0f, logo.pos.y + 436.0f);
	cursol1.tex = DirectX::XMFLOAT2(0.0f, 1162.0f);
	cursol1.size = DirectX::XMFLOAT2(150.0f, 150.0f);

	cursol2.pos = DirectX::XMFLOAT2(660.0f + 150.0f, logo.pos.y + 436.0f);
	cursol2.tex = DirectX::XMFLOAT2(0.0f, 1312.0f);
	cursol2.size = DirectX::XMFLOAT2(150.0f, 150.0f);

	ok1.pos = DirectX::XMFLOAT2((char1.pos.x + char1.size.x / 2.0f) - (138.0f / 2.0f),
		(char1.pos.y + (char1.size.y / 2.0f)) - (char1.size.y / 4.0f));
	ok1.tex = DirectX::XMFLOAT2(0.0f, 1462.0f);
	ok1.size = DirectX::XMFLOAT2(138.0f, 72.0f);

	ok2.pos = DirectX::XMFLOAT2((char2.pos.x + char2.size.x / 2.0f) - (138.0f / 2.0f),
		(char2.pos.y + (char2.size.y / 2.0f)) - (char2.size.y / 4.0f));
	ok2.tex = DirectX::XMFLOAT2(138.0f, 1462.0f);
	ok2.size = DirectX::XMFLOAT2(138.0f, 72.0f);

	isMove = true;

	isCharacter1 = true;
	isCharacter2 = false;
	isCharacter3 = false;
	isCharacter4 = false;

	isCharacter1_2 = false;
	isCharacter2_2 = true;
	isCharacter3_2 = false;
	isCharacter4_2 = false;

	rightCnt = 0;
	leftCnt = 0;

	rightCnt2 = 0;
	leftCnt2 = 0;

	isDecisionOnePlayer = false;
	isDecisionTwoPlayer = false;

	isOkOnePlayer = false;
	isOkTwoPlayer = false;

	twoPlayerCnt = 0;

	onePlayerCnt_B = 0;
	twoPlayerCnt_B = 0;

	moveCnt1 = 0;
	moveCnt2 = 0;

	alpha = 1.0f;
}

void SceneSelect::CharSelectUpdate()
{
	if (!isMove)
	{
		if (!isNextMove)
		{
			switch (gameMode)
			{
			case ONE_PLAYER:
				if (!isDecisionOnePlayer) OnePlayerUpdate();
				else
				{
					ok1.pos.y = easing::OutExp(moveCnt1, CNT_MAX_4, 366.0f, char1.pos.y + (char1.size.y / 2.0f) - (char1.size.y / 4.0f));
					if (CNT_MAX_4 <= moveCnt1++)
					{
						//selectState = SelectState::LEVEL_SELECT;
						isNextMove = true;
						moveCnt1 = 0;
					}
				}
				break;
			case TWO_PLAYER:
				if (!isDecisionOnePlayer) OnePlayerUpdate();
				else if (!isOkOnePlayer)
				{
					ok1.pos.y = easing::OutExp(moveCnt1, CNT_MAX_4, 366.0f, char1.pos.y + (char1.size.y / 2.0f) - (char1.size.y / 4.0f));
					if (CNT_MAX_4 <= moveCnt1++)
					{
						//selectState = SelectState::LEVEL_SELECT;
						moveCnt1 = 0;
						isOkOnePlayer = true;
					}
				}
				if (!isDecisionTwoPlayer) TwoPlayerUpdate();
				else if (!isOkTwoPlayer)
				{
					ok2.pos.y = easing::OutExp(moveCnt2, CNT_MAX_4, 366.0f, char2.pos.y + (char2.size.y / 2.0f) - (char2.size.y / 4.0f));
					if (CNT_MAX_4 <= moveCnt2++)
					{
						//selectState = SelectState::LEVEL_SELECT;
						moveCnt2 = 0;
						isOkTwoPlayer = true;
					}
				}

				if (isOkOnePlayer && isOkTwoPlayer)
				{
					isNextMove = true;
				}
				else if (isOkOnePlayer && !isOkTwoPlayer)
				{
					if (pad[0].bBs)
					{
						if (onePlayerCnt_B == 0)
						{
							isDecisionOnePlayer = false;
							isOkOnePlayer = false;
							cursol1.tex = DirectX::XMFLOAT2(0.0f, 1162.0f);
							ok1.pos = DirectX::XMFLOAT2((char1.pos.x + char1.size.x / 2.0f) - (138.0f / 2.0f),
								(char1.pos.y + (char1.size.y / 2.0f)) - (char1.size.y / 4.0f));
							ok1.tex = DirectX::XMFLOAT2(0.0f, 1462.0f);
						}
						onePlayerCnt_B++;
					}
					else
					{
						onePlayerCnt_B = 0;
					}
				}
				else if (!isOkOnePlayer && isOkTwoPlayer)
				{
					if (pad[1].bBs)
					{
						isDecisionTwoPlayer = false;
						isOkTwoPlayer = false;
						cursol2.tex = DirectX::XMFLOAT2(0.0f, 1312.0f);
						ok2.pos = DirectX::XMFLOAT2((char2.pos.x + char2.size.x / 2.0f) - (138.0f / 2.0f),
							(char2.pos.y + (char2.size.y / 2.0f)) - (char2.size.y / 4.0f));
						ok2.tex = DirectX::XMFLOAT2(138.0f, 1462.0f);
					}
				}
				break;
			}
		}
		else if (isNextMove)
		{
			logo.pos.y = easing::OutExp(cnt, CNT_MAX_2, -600.0f - 21.0f, 72.0f);
			charList.pos.y = easing::OutExp(cnt, CNT_MAX_2, -164.0f - 21.0f, 504.0f);
			cursol1.pos.y = easing::OutExp(cnt, CNT_MAX_2,  -164.0f - 21.0f, 504.0f);
			cursol2.pos.y = easing::OutExp(cnt, CNT_MAX_2,  -164.0f - 21.0f, 504.0f);

			if (55 <= cnt)
			{
				alpha -= 1.0f / 5.0f;
			}
			if (CNT_MAX_3 <= cnt++)
			{
				cnt = 0;
				isNextMove = false;
				logo.pos.y = -600.0f;
				alpha = 0.0f;
				selectState = SelectState::LEVEL_SELECT;
			}
		}
	}
	else
	{
		logo.pos.y = easing::OutExp(cnt, CNT_MAX_2, 72.0f + 21.0f, -600.0f);
		charList.pos.y = easing::OutExp(cnt, CNT_MAX_2, 504.0f + 21.0f, -164.0f);
		cursol1.pos.y  = easing::OutExp(cnt, CNT_MAX_2, 504.0f + 21.0f, -164.0f);
		cursol2.pos.y  = easing::OutExp(cnt, CNT_MAX_2, 504.0f + 21.0f, -164.0f);

		if (CNT_MAX_3 <= cnt++)
		{
			cnt = 0;
			isMove = false;
			logo.pos.y = 72.0f;
			charList.pos.y = 504.0f;
			cursol1.pos.y = 504.0f;
			cursol2.pos.y = 504.0f;
		}
	}
}

void SceneSelect::CharSelectDraw()
{
	DirectX::XMFLOAT4 nomalColor(1.0f, 1.0f, 1.0f, 1.0f);

	sprSelectUI->Begin();

	sprSelectUI->Draw(charList.pos, charList.size, charList.tex, charList.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	sprSelectUI->Draw(cursol1.pos, cursol1.size, cursol1.tex, cursol1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	if (isDecisionOnePlayer)
	{
		sprSelectUI->Draw(ok1.pos, ok1.size, ok1.tex, ok1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
	}
	if (gameMode == GameMode::TWO_PLAYER)
	{
		sprSelectUI->Draw(cursol2.pos, cursol2.size, cursol2.tex, cursol2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
		if (isDecisionTwoPlayer)
		{
			sprSelectUI->Draw(ok2.pos, ok2.size, ok2.tex, ok2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
		}
	}
	
	sprSelectUI->End();
}

// キャラクターセレクト用 更新関数
void SceneSelect::OnePlayerUpdate()
{
	if (pad[0].bAt)
	{
		pAudio->Play(SOUND->seHandle[SOUND->OK]);
		cursol1.tex = DirectX::XMFLOAT2(150.0f, 1162.0f);
		isDecisionOnePlayer = true;
	}

	if (pad[0].bRIGHTs || 0 < pad[0].sX)
	{
		if (++rightCnt == 1)
		{
			if (characterNum < CharacterType::CHARACTER_4)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				characterNum++;
			}
		}
		if (rightCnt >= ACCEL_START)
		{
			if (rightCnt % ACCEL_SPEED_PAR == 0)
			{
				if (characterNum < CharacterType::CHARACTER_4)
				{
					pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
					characterNum++;
				}
			}
		}
	}
	else
	{
		rightCnt = 0;
	}

	if (pad[0].bLEFTs || pad[0].sX < 0)
	{
		if (++leftCnt == 1)
		{
			if (CharacterType::CHARACTER_1 < characterNum)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				characterNum--;
			}
		}
		if (leftCnt >= ACCEL_START)
		{
			if (leftCnt % ACCEL_SPEED_PAR == 0)
			{
				if (CharacterType::CHARACTER_1 < characterNum)
				{
					pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
					characterNum--;
				}
			}
		}
	}
	else
	{
		leftCnt = 0;
	}

	switch (characterNum)
	{
	case CharacterType::CHARACTER_1:
		isCharacter1 = true;
		isCharacter2 = false;
		isCharacter3 = false;
		isCharacter4 = false;
		break;
	case CharacterType::CHARACTER_2:
		isCharacter1 = false;
		isCharacter2 = true;
		isCharacter3 = false;
		isCharacter4 = false;
		break;
	case CharacterType::CHARACTER_3:
		isCharacter1 = false;
		isCharacter2 = false;
		isCharacter3 = true;
		isCharacter4 = false;
		break;
	case CharacterType::CHARACTER_4:
		isCharacter1 = false;
		isCharacter2 = false;
		isCharacter3 = false;
		isCharacter4 = true;
		break;
	}

	if (isCharacter1) cursol1.pos.x = 660.0f;
	else if (isCharacter2) cursol1.pos.x = 660.0f + 150.0f;
	else if (isCharacter3) cursol1.pos.x = 660.0f + 300.0f;
	else if (isCharacter4) cursol1.pos.x = 660.0f + 450.0f;
}

void SceneSelect::TwoPlayerUpdate()
{
#ifdef DEBUG_TWOPLAY_OF_ONE_PAD
	if (InputTrigger(XINPUT_A))
	{
		cursol2.tex = DirectX::XMFLOAT2(150.0f, 1312.0f);
		isDecisionTwoPlayer = true;
	}

	/*if (InputTrigger(XINPUT_DPAD_RIGHT) || DPadInputState(STICK_RIGHT))
	{
		if (characterNum2 < CharacterType::CHARACTER_4) characterNum2++;
	}
	if (InputTrigger(XINPUT_DPAD_LEFT) || DPadInputState(STICK_LEFT))
	{
		if (CharacterType::CHARACTER_1 < characterNum2) characterNum2--;
	}*/
#else
	if (pad[1].bAt)
	{
		pAudio->Play(SOUND->seHandle[SOUND->OK]);
		cursol2.tex = DirectX::XMFLOAT2(150.0f, 1312.0f);
		isDecisionTwoPlayer = true;
	}

	if (pad[1].bRIGHTs || 0 < pad[1].sX)
	{
		if (++rightCnt2 == 1)
		{
			if (characterNum2 < CharacterType::CHARACTER_4)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				characterNum2++;
			}
		}
		if (rightCnt2 >= ACCEL_START)
		{
			if (rightCnt2 % ACCEL_SPEED_PAR == 0)
			{
				if (characterNum2 < CharacterType::CHARACTER_4)
				{
					pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
					characterNum2++;
				}
			}
		}
	}
	else
	{
		rightCnt2 = 0;
	}

	if (pad[1].bLEFTs || pad[1].sX < 0)
	{
		if (++leftCnt2 == 1)
		{
			if (CharacterType::CHARACTER_1 < characterNum2)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				characterNum2--;
			}
		}
		if (leftCnt2 >= ACCEL_START)
		{
			if (leftCnt2 % ACCEL_SPEED_PAR == 0)
			{
				if (CharacterType::CHARACTER_1 < characterNum2)
				{
					pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
					characterNum2--;
				}
			}
		}
	}
	else
	{
		leftCnt2 = 0;
	}

#endif // DEBUG_TWOPLAY_OF_ONE_PAD

	switch (characterNum2)
	{
	case CharacterType::CHARACTER_1:
		isCharacter1_2 = true;
		isCharacter2_2 = false;
		isCharacter3_2 = false;
		isCharacter4_2 = false;
		break;
	case CharacterType::CHARACTER_2:
		isCharacter1_2 = false;
		isCharacter2_2 = true;
		isCharacter3_2 = false;
		isCharacter4_2 = false;
		break;
	case CharacterType::CHARACTER_3:
		isCharacter1_2 = false;
		isCharacter2_2 = false;
		isCharacter3_2 = true;
		isCharacter4_2 = false;
		break;
	case CharacterType::CHARACTER_4:
		isCharacter1_2 = false;
		isCharacter2_2 = false;
		isCharacter3_2 = false;
		isCharacter4_2 = true;
		break;
	}

	if (isCharacter1_2) cursol2.pos.x = 660.0f;
	else if (isCharacter2_2) cursol2.pos.x = 660.0f + 150.0f;
	else if (isCharacter3_2) cursol2.pos.x = 660.0f + 300.0f;
	else if (isCharacter4_2) cursol2.pos.x = 660.0f + 450.0f;
}

// レベルセレクト
void SceneSelect::levelSelectInit()
{
	logo.tex = DirectX::XMFLOAT2(0.0f, 320.0f);

	difficultyNum = DifficultyType::DIFFICULTY_4;
	colorNum = difficultyColorNum[difficultyNum];
	secondNum = dificultyGaugeSpeedNum[difficultyNum];

	difficultyNum2 = DifficultyType::DIFFICULTY_4;
	colorNum2 = difficultyColorNum[difficultyNum2];
	secondNum2 = dificultyGaugeSpeedNum[difficultyNum];

	ok1.pos = DirectX::XMFLOAT2((char1.pos.x + char1.size.x / 2.0f) - (138.0f / 2.0f),
		(char1.pos.y + (char1.size.y / 2.0f)) - (char1.size.y / 4.0f));
	ok1.tex = DirectX::XMFLOAT2(0.0f, 1462.0f);
	ok1.size = DirectX::XMFLOAT2(138.0f, 72.0f);

	ok2.pos = DirectX::XMFLOAT2((char2.pos.x + char2.size.x / 2.0f) - (138.0f / 2.0f),
		(char2.pos.y + (char2.size.y / 2.0f)) - (char2.size.y / 4.0f));
	ok2.tex = DirectX::XMFLOAT2(138.0f, 1462.0f);
	ok2.size = DirectX::XMFLOAT2(138.0f, 72.0f);

	switch (gameMode)
	{
	case ONE_PLAYER:
		logo.pos.y = -688.0f;
		levelList1.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 432.0f);
		levelList1.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f);
		levelList1.size = DirectX::XMFLOAT2(840.0f, 258.0f);
		break;
	case TWO_PLAYER:
		levelList1.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 258.0f);
		levelList1.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f);
		levelList1.size = DirectX::XMFLOAT2(840.0f, 258.0f);

		levelList2.pos = DirectX::XMFLOAT2(544.0f, 1182.0f);
		levelList2.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f + 258.0f);
		levelList2.size = DirectX::XMFLOAT2(840.0f, 258.0f);
		break;
	}

	//levelListFilter1.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 432.0f);
	levelListFilter1.tex = DirectX::XMFLOAT2(0.0f, 1462.0f + 72.0f + 258.0f * 2);
	levelListFilter1.size = DirectX::XMFLOAT2(840.0f, 258.0f);

	//levelListFilter2.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 432.0f);
	levelListFilter2.tex = DirectX::XMFLOAT2(0.0f, 1462.0f + 72.0f + 258.0f * 3);
	levelListFilter2.size = DirectX::XMFLOAT2(840.0f, 258.0f);

	alpha = 1.0f;

	isMove = true;

	isDecisionOnePlayer2 = false;
	isDecisionTwoPlayer2 = false;

	isOkOnePlayer2 = false;
	isOkTwoPlayer2 = false;

	rightCnt_2 = 0;
	leftCnt_2 = 0;

	rightCnt2_2 = 0;
	leftCnt2_2 = 0;
}

void SceneSelect::levelSelectUpdate()
{
	if (!isMove)
	{
		switch (gameMode)
		{
		case ONE_PLAYER:
			if (!isDecisionOnePlayer2) OnePlayerLevelUpdate();
			else
			{
				ok1.pos.y = easing::OutExp(moveCnt1, CNT_MAX_4, 366.0f, char1.pos.y + (char1.size.y / 2.0f) - (char1.size.y / 4.0f));
				if (CNT_MAX_4 <= moveCnt1++)
				{
					PRODUCTION->SetOn(GO_GAME);
					PRODUCTION->Start();
					moveCnt1 = 0;
				}
			}
			break;
		case TWO_PLAYER:
			if (!isDecisionOnePlayer2) OnePlayerLevelUpdate();
			else if (!isOkOnePlayer2)
			{
				ok1.pos.y = easing::OutExp(moveCnt1, CNT_MAX_4, 366.0f, char1.pos.y + (char1.size.y / 2.0f) - (char1.size.y / 4.0f));
				if (CNT_MAX_4 <= moveCnt1++)
				{
					moveCnt1 = 0;
					isOkOnePlayer2 = true;
				}
			}
			if (!isDecisionTwoPlayer2) TwoPlayerLevelUpdate();
			else if (!isOkTwoPlayer2)
			{
				ok2.pos.y = easing::OutExp(moveCnt2, CNT_MAX_4, 366.0f, char2.pos.y + (char2.size.y / 2.0f) - (char2.size.y / 4.0f));
				if (CNT_MAX_4 <= moveCnt2++)
				{
					moveCnt2 = 0;
					isOkTwoPlayer2 = true;
				}
			}

			if (isOkOnePlayer2 && isOkTwoPlayer2)
			{
				PRODUCTION->SetOn(GO_GAME);
				PRODUCTION->Start();
			}
			else if (isOkOnePlayer2 && !isOkTwoPlayer2)
			{
				if (pad[0].bBt)
				{
					isDecisionOnePlayer2 = false;
					isOkOnePlayer2 = false;
					ok1.pos = DirectX::XMFLOAT2((char1.pos.x + char1.size.x / 2.0f) - (138.0f / 2.0f),
						(char1.pos.y + (char1.size.y / 2.0f)) - (char1.size.y / 4.0f));
					ok1.tex = DirectX::XMFLOAT2(0.0f, 1462.0f);
				}
			}
			else if (!isOkOnePlayer2 && isOkTwoPlayer2)
			{
				if (pad[1].bBt)
				{
					isDecisionTwoPlayer2 = false;
					isOkTwoPlayer2 = false;
					ok2.pos = DirectX::XMFLOAT2((char2.pos.x + char2.size.x / 2.0f) - (138.0f / 2.0f),
						(char2.pos.y + (char2.size.y / 2.0f)) - (char2.size.y / 4.0f));
					ok2.tex = DirectX::XMFLOAT2(138.0f, 1462.0f);
				}
			}
			break;
		}
	}
	else
	{
		switch (gameMode)
		{
		case ONE_PLAYER:
			logo.pos.y = easing::OutExp(cnt, CNT_MAX_2, 72.0f + 22.5f, -688.0f);
			levelList1.pos.y = easing::OutExp(cnt, CNT_MAX_2, 504.0f + 22.5f, -256.0f);
			break;
		case TWO_PLAYER:
			logo.pos.y = easing::OutExp(cnt, CNT_MAX_2, 72.0f + 22.5f, -600.0f);
			levelList1.pos.y = easing::OutExp(cnt, CNT_MAX_2, 330.0f + 22.5f, -342.0f);
			levelList2.pos.y = easing::OutExp(cnt, CNT_MAX_2, 654.0f - 22.5f, 1182.0f);
			break;
		}

		if (CNT_MAX_3 <= cnt++)
		{
			cnt = 0;
			isMove = false;
			logo.pos.y = 72.0f;
			switch (gameMode)
			{
			case ONE_PLAYER:
				levelList1.pos.y = 504.0f;
				break;
			case TWO_PLAYER:
				levelList1.pos.y = 330.0f;
				levelList2.pos.y = 654.0f;
				break;
			}
		}
	}
}

void SceneSelect::levelSelectDraw()
{
	DirectX::XMFLOAT4 nomalColor(1.0f, 1.0f, 1.0f, 1.0f);

	sprSelectUI->Begin();

	sprSelectUI->Draw(levelList1.pos, levelList1.size, levelList1.tex, levelList1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	if (isDecisionOnePlayer2)
	{
		sprSelectUI->Draw(levelListFilter1.pos, levelListFilter1.size, levelListFilter1.tex, levelListFilter1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
	}
	if (gameMode == GameMode::TWO_PLAYER)
	{
		sprSelectUI->Draw(levelList2.pos, levelList2.size, levelList2.tex, levelList2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
		if (isDecisionTwoPlayer2)
		{
			sprSelectUI->Draw(levelListFilter2.pos, levelListFilter2.size, levelListFilter2.tex, levelListFilter2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
		}
	}

	if (isDecisionOnePlayer)
	{
		sprSelectUI->Draw(ok1.pos, ok1.size, ok1.tex, ok1.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
	}
	if (gameMode == GameMode::TWO_PLAYER)
	{
		sprSelectUI->Draw(cursol2.pos, cursol2.size, cursol2.tex, cursol2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, nomalColor);
		if (isDecisionTwoPlayer)
		{
			sprSelectUI->Draw(ok2.pos, ok2.size, ok2.tex, ok2.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
		}
	}

	sprSelectUI->End();
}

// レベルセレクト用 更新関数
void SceneSelect::OnePlayerLevelUpdate()
{
	if (pad[0].bAt)
	{
		pAudio->Play(SOUND->seHandle[SOUND->OK]);
		levelListFilter1.pos = levelList1.pos;
		isDecisionOnePlayer2 = true;
	}


	if (pad[0].bRIGHTs || 0 < pad[0].sX)
	{
		if (++rightCnt_2 == 1)
		{
			if (difficultyNum < DifficultyType::DIFFICULTY_7)
			{
				pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
				difficultyNum++;
			}
		}
		if (rightCnt_2 >= ACCEL_START)
		{
			if (rightCnt_2 % ACCEL_SPEED_PAR == 0)
			{
				if (difficultyNum < DifficultyType::DIFFICULTY_7)
				{
					pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
					difficultyNum++;
				}
			}
		}
	}
	else
	{
		rightCnt_2 = 0;
	}

	if (pad[0].bLEFTs || pad[0].sX < 0)
	{
		if (++leftCnt_2 == 1)
		{
			if (DifficultyType::DIFFICULTY_1 < difficultyNum)
			{
				pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
				difficultyNum--;
			}
		}
		if (leftCnt_2 >= ACCEL_START)
		{
			if (leftCnt_2 % ACCEL_SPEED_PAR == 0)
			{
				if (DifficultyType::DIFFICULTY_1 < difficultyNum)
				{
					pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
					difficultyNum--;
				}
			}
		}
	}
	else
	{
		leftCnt_2 = 0;
	}

	levelList1.tex.x = 840.0f * difficultyNum;

	colorNum = difficultyColorNum[difficultyNum];
	secondNum = dificultyGaugeSpeedNum[difficultyNum];
}

void SceneSelect::TwoPlayerLevelUpdate()
{
#ifdef DEBUG_TWOPLAY_OF_ONE_PAD
	if (InputTrigger(XINPUT_A))
	{
		levelListFilter2.pos = levelList2.pos;
		isDecisionTwoPlayer2 = true;
	}

	/*if (InputTrigger(XINPUT_DPAD_LEFT, 1))
	{
		if (difficultyNum2 < DifficultyType::DIFFICULTY_7) difficultyNum2++;
	}
	if (InputTrigger(XINPUT_DPAD_RIGHT, 1))
	{
		if (DifficultyType::DIFFICULTY_1 < difficultyNum2) difficultyNum2--;
	}*/
#else
	if (pad[1].bAt)
	{
		pAudio->Play(SOUND->seHandle[SOUND->OK]);
		levelListFilter2.pos = levelList2.pos;
		isDecisionTwoPlayer2 = true;
	}

	if (pad[1].bRIGHTs || 0 < pad[1].sX)
	{
		if (++rightCnt2_2 == 1)
		{
			if (difficultyNum2 < DifficultyType::DIFFICULTY_7)
			{
				pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
				difficultyNum2++;
			}
		}
		if (rightCnt2_2 >= ACCEL_START)
		{
			if (rightCnt2_2 % ACCEL_SPEED_PAR == 0)
			{
				if (difficultyNum2 < DifficultyType::DIFFICULTY_7)
				{
					pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
					difficultyNum2++;
				}
			}
		}
	}
	else
	{
		rightCnt2_2 = 0;
	}

	if (pad[1].bLEFTs || pad[1].sX < 0)
	{
		if (++leftCnt2_2 == 1)
		{
			if (DifficultyType::DIFFICULTY_1 < difficultyNum2)
			{
				pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
				difficultyNum2--;
			}
		}
		if (leftCnt2_2 >= ACCEL_START)
		{
			if (leftCnt2_2 % ACCEL_SPEED_PAR == 0)
			{
				if (DifficultyType::DIFFICULTY_1 < difficultyNum2)
				{
					pAudio->Play(SOUND->seHandle[SOUND->LEVEL_SELECT]);
					difficultyNum2--;
				}
			}
		}
	}
	else
	{
		leftCnt2_2 = 0;
	}
#endif // DEBUG_TWOPLAY_OF_ONE_PAD

	levelList2.tex.x = 840.0f * difficultyNum2;

	colorNum2 = difficultyColorNum[difficultyNum2];
	secondNum2 = dificultyGaugeSpeedNum[difficultyNum2];
}

void SceneSelect::Uninit()
{
	LevelSelectUnInit();
}

void SceneSelect::LevelSelectUnInit()
{
	switch (gameMode)
	{
	case ONE_PLAYER:
		logo.pos.y = -688.0f;
		levelList1.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 432.0f);
		levelList1.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f);
		levelList1.size = DirectX::XMFLOAT2(840.0f, 258.0f);
		break;
	case TWO_PLAYER:
		logo.pos.y = -600.0f;
		levelList1.pos = DirectX::XMFLOAT2(544.0f, logo.pos.y + 258.0f);
		levelList1.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f);
		levelList1.size = DirectX::XMFLOAT2(840.0f, 258.0f);

		levelList2.pos = DirectX::XMFLOAT2(544.0f, 1182.0f);
		levelList2.tex = DirectX::XMFLOAT2(840.0f * difficultyNum, 1462.0f + 72.0f + 258.0f);
		levelList2.size = DirectX::XMFLOAT2(840.0f, 258.0f);
		break;
	}

	isDecisionOnePlayer2 = false;
	isDecisionTwoPlayer2 = false;

	pAudio->Stop(SOUND->bgmHandle[SOUND->SELECT]);
	pAudio->DeleteSourceVoice(SOUND->bgmHandle[SOUND->SELECT]);
}