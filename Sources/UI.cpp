#include "UI.h"
#include "SceneManager.h"
#include "GameMaster.h"
#include "BlockManager.h"
#include "easing.h"
#include "Sound.h"
#include "Blender.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#define SPEED_UP_CNT_MAX	3 //(秒)
#define SPEED_UP_POS_Y 468.0f
#define METER_POS_Y    510.0f
#define GAGE_DOWN_CNT_MAX  60

void UI::Init(int _playerNum)
{
	sprUI = std::make_unique<SpriteBatch>(L"Data/UI.png", 100, DirectX::XMFLOAT2(0.0f, 0.0f));


	// TODO : レベルセレクトからデータをもらう
	if (!sceneGame.isTwoPlayer)
	{
		if (_playerNum == GameMode::ONE_PLAYER)
		{
			maxSecond = static_cast<float>(sceneSelect.secondNum);
		}
		else if (_playerNum == GameMode::TWO_PLAYER)
		{
			maxSecond = static_cast<float>(sceneSelect.secondNum2);
		}
	}
	else
	{
		if (_playerNum == GameMode::ONE_PLAYER)
		{
			maxSecond = static_cast<float>(sceneSelect.secondNum) * 2;
		}
		else if (_playerNum == GameMode::TWO_PLAYER)
		{
			maxSecond = static_cast<float>(sceneSelect.secondNum2) * 2;
		}

	}

	if (!sceneGame.isTwoPlayer)
	{
		// メーター
		meter.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
		meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
		meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);

		// メーター背景
		meterBack.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
		meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
		meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

		// speedUP!
		speedUp.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
		speedUp.tex = DirectX::XMFLOAT2(0.0f, 2292.0f);
		speedUp.size = DirectX::XMFLOAT2(136.0f, 28.0f);

		// 矢印
		arrow.pos = DirectX::XMFLOAT2(1302.0f + 36.0f + ADJUST, 552.0f);
		arrow.tex = DirectX::XMFLOAT2(0.0f, 2076.0f);
		arrow.size = DirectX::XMFLOAT2(46.0f, 108.0f);
	}
	else
	{
		switch (_playerNum)
		{
		case 0:
			// メーター
			meter.pos = DirectX::XMFLOAT2(711.0f - ADJUST * 2.0f, 510.0f);
			meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
			meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);

			// メーター背景
			meterBack.pos = DirectX::XMFLOAT2(711.0f - ADJUST * 2.0f, 510.0f);
			meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
			meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

			// speedUP!
			speedUp.pos = DirectX::XMFLOAT2(711.0f - ADJUST * 2.0f, 468.0f);
			speedUp.tex = DirectX::XMFLOAT2(0.0f, 2292.0f);
			speedUp.size = DirectX::XMFLOAT2(136.0f, 28.0f);

			// 矢印
			arrow.pos = DirectX::XMFLOAT2(711.0f + 36.0f - ADJUST * 2.0f, 552.0f);
			arrow.tex = DirectX::XMFLOAT2(0.0f, 2076.0f);
			arrow.size = DirectX::XMFLOAT2(46.0f, 108.0f);
			break;
		case 1:
			// メーター
			meter.pos = DirectX::XMFLOAT2(1173.0f + ADJUST * 2.0f, 510.0f);
			meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
			meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);

			// メーター背景
			meterBack.pos = DirectX::XMFLOAT2(1173.0f + ADJUST * 2.0f, 510.0f);
			meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
			meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

			// speedUP!
			speedUp.pos = DirectX::XMFLOAT2(1173.0f + 36.0f - 136.0f + ADJUST * 2.0f, 468.0f);
			speedUp.tex = DirectX::XMFLOAT2(0.0f, 2292.0f + 28.0f);
			speedUp.size = DirectX::XMFLOAT2(136.0f, 28.0f);

			// 矢印
			arrow.pos = DirectX::XMFLOAT2(1173.0f - 46.0f + ADJUST * 2.0f, 552.0f);
			arrow.tex = DirectX::XMFLOAT2(0.0f, 2076.0f + 108.0f);
			arrow.size = DirectX::XMFLOAT2(46.0f, 108.0f);
			break;
		}
	}

	if (!sceneGame.isTwoPlayer)
	{
		for (int i = 0; i < 6; i++)
		{
			block[i].pos = DirectX::XMFLOAT2(3.0f + 615.0f + 114.0f * i, 12.0f + 1032.0f);
			rowColor[i] = -1;
		}
	}
	else
	{
		switch (_playerNum)
		{
		case 0:
			for (int i = 0; i < 6; i++)
			{
				block[i].pos = DirectX::XMFLOAT2(ADJUST + TWO_PLAY_ONE_ORIJIN_X + 114.0f * i, 12.0f + 1032.0f);
				rowColor[i] = -1;
			}
			break;
		case 1:
			for (int i = 0; i < 6; i++)
			{
				block[i].pos = DirectX::XMFLOAT2(ADJUST + TWO_PLAY_TWO_ORIJIN_X + 114.0f * i, 12.0f + 1032.0f);
				rowColor[i] = -1;
			}
			break;
		}
	}

	// block
	// TODO : for (int i = 0; i < 4; i++)  ->  for (int i = 0; i < difficltySelect.colorNum; i++)
	for (int i = 0; i < GAME_MASTER->blockManager[_playerNum].colorMax; i++)
	{
		block[i].tex = DirectX::XMFLOAT2(114.0f * (i + 1), 228.0f);
		block[i].size = DirectX::XMFLOAT2(114.0f, 228.0f);
	}

	// ScoreBoard
	if (!sceneGame.isTwoPlayer)
	{
		scoreBoard.pos = DirectX::XMFLOAT2(1416.0f, 774.0f);
		scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
		scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

		scorePos = DirectX::XMFLOAT2(1463.0f, 879.0f);
	}
	else
	{
		switch (_playerNum)
		{
		case 0:
			// 1P
			scoreBoard.pos = DirectX::XMFLOAT2(804.0f, 744.0f);
			scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
			scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

			scorePos = DirectX::XMFLOAT2(852.0f, 849.0f);
			break;
		case 1:
			// 2P
			scoreBoard.pos = DirectX::XMFLOAT2(804.0f, 215.0f);
			scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f + 192.0f);
			scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

			scorePos = DirectX::XMFLOAT2(852.0f, 319.0f);
			break;
		}
	}

	// Chain Display
	chainPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	chainFirstPosY = 0.0f;
	chainDisplayTimer = 0;
	isDisplayChain = false;
	chainDisplayAlpha = 0.0f;

	totalTime = 0;
	minTime = 0;
	secondTime = 0;
	speedUpCount = 0;

	isTimerStop = false;

	second = 0.0f;
//	if (!sceneGame.isTwoPlayer)
//	{
//		maxSecond = 6.5f; // ゲージ一周あたりの秒数
//	}
//	else
//	{
//		maxSecond = 13.0f;
//	}
	interpolationTimeCnt = 0;
	totalScore = 0;

	isSpeedUp = false;
	textSpeedUpCnt = 0;

	alpha = 0.0f;
	gageDownCnt = 0;
	isGageMax = false;

	readyTimer = 0;
	readyState = 0;
	readyAlpha = 0.0f;
	readyPosX = 660.0f;
	readyPosY = 462.0f;
	isDisplayGo = 0.0f;

	tPosY = meter.tex.y + (maxSecond - second) / maxSecond * meter.size.y;
	if (tPosY < 951.0f)
	{
		tPosY = 936.0f + 570.0f;
		if (_playerNum == 1) meter.tex.x = 36.0f;
		meter.tex.y = 936.0f + 570.0f;
	}
	else if (maxSecond <= second && tPosY <= 936.0f + 570.0f)
	{
		tPosY = 936.0f + 570.0f;
	}
	tSizeY = meter.size.y - (tPosY - meter.tex.y);

	posY = meter.pos.y + (meter.size.y - tSizeY);

	winLoseAlpha = 0.0f;
	isWinLoseUp = true;
}

void UI::Update(int _playerNum)
{
	for (int i = 0; i < 6; i++)
	{
		for (size_t k = 0; k < GAME_MASTER->blockManager[_playerNum].block.size(); k++)
		{
			if (GAME_MASTER->blockManager[_playerNum].block[k].row == i && GAME_MASTER->blockManager[_playerNum].block[k].column == 9)
			{
				rowColor[i] = GAME_MASTER->blockManager[_playerNum].block[k].color;
			}
		}
	}

	if (sceneGame.isGameOver_one || sceneGame.isGameOver_two) return;

	//Ready Go のくだり
	if (sceneGame.isStartInit)
	{
//		if (_playerNum != 0) return;
		switch (readyState)
		{
		case 0:
			++readyTimer;
			if (readyTimer == 50)
			{
				pAudio->Play(SOUND->seHandle[SOUND->READY]);
			}
			if (readyTimer <= 60 && readyTimer>=50)
			{
				readyAlpha += 0.1f;
			}
			if (readyTimer >= 50)
				readyPosY = easing::OutExp(static_cast<float>(readyTimer), 40.0f, 462.0f, 462.0f + 114.0f);
			if (readyTimer >= 180)
			{
				readyState++;
				readyTimer = 0;
				readyAlpha = 0.0f;
			}
			break;
		case 1:
			++readyTimer;
			if (readyTimer == 1)
			{
				pAudio->Play(SOUND->seHandle[SOUND->GO]);
			}

			if (++readyTimer <= 60)
			{
				readyAlpha += 0.1f;
			}
			if (readyTimer >= 50)
				readyPosY = easing::OutExp(static_cast<float>(readyTimer), 40.0f, 462.0f, 462.0f + 114.0f);
			if (readyTimer >= 120)
			{
				readyState++;
				readyTimer = 0;
				readyAlpha = 0.0f;
			}
			break;
		}
	}


/*	for (int i = 0; i < 6; i++)
	{
		for (size_t k = 0; k < GAME_MASTER->blockManager[_playerNum].block.size(); k++)
		{
			if (GAME_MASTER->blockManager[_playerNum].block[k].row == i && GAME_MASTER->blockManager[_playerNum].block[k].column == 9)
			{
				rowColor[i] = GAME_MASTER->blockManager[_playerNum].block[k].color;
			}
		}
	} */

	if (sceneGame.isStartInit)return;

	if (!isTimerStop)
	{
		second = interpolationTimeCnt / 60.0f;
		//if (meter.tex.y < 951.0f)
		//{
		//	meter.tex.y = 936.0f + 570.0f;
		//}
		if (maxSecond + 1 < second)
		{
			isTimerStop = true;
			isGageMax = true;
			gageDownCnt = 0;
			GAME_MASTER->blockManager[_playerNum].isTimePushUpBoard = true;
			second = 0;
			interpolationTimeCnt = -1;
			//TODO:ここでフラグとめる
		}
		interpolationTimeCnt++;
	}
	if (isGageMax)
	{
		if (GAGE_DOWN_CNT_MAX <= gageDownCnt)
		{
			second = 0;
		}
		else
		{
			second = easing::OutExp(static_cast<float>(gageDownCnt), static_cast<float>(GAGE_DOWN_CNT_MAX), 0.0f, maxSecond);
		}
		gageDownCnt++;
	}

	tPosY = meter.tex.y + (maxSecond - second) / maxSecond * meter.size.y;
	if (tPosY < 951.0f)
	{
		pAudio->Play(SOUND->seHandle[SOUND->SPEED_UP]);
		tPosY = 936.0f + 570.0f;
		if (_playerNum == 1) meter.tex.x = 36.0f;
		meter.tex.y = 936.0f + 570.0f;
	}
	else if (maxSecond <= second && tPosY <= 936.0f + 570.0f)
	{
		tPosY = 936.0f + 570.0f;
	}
	tSizeY = meter.size.y - (tPosY - meter.tex.y);

	posY = meter.pos.y + (meter.size.y - tSizeY);


	if (totalTime / 60)	// 秒
	{
		secondTime++;
		totalTime = 0;
	}
	if (secondTime / 60) // 分
	{
		secondTime = 0;
		minTime++;
	}

	if (speedUpCount == 0)
	{
		if (secondTime >= 45 && secondTime != 0)
		{
			if (!sceneGame.isTwoPlayer)
			{
				maxSecond -= 0.5f;
				pAudio->Play(SOUND->seHandle[SOUND->GAUGE_MAX]);
			}
			else
			{
				maxSecond -= 0.6f;
			}
			speedUpCount++;
			secondTime = 0;
			isSpeedUp = true;
		}
	}
	else
	{
		if (secondTime >= 30 && secondTime != 0)
		{
			if (!sceneGame.isTwoPlayer)
			{
				maxSecond -= 0.5f;
			}
			else
			{
				maxSecond -= 0.6f;
			}
			speedUpCount++;
			secondTime = 0;
			isSpeedUp = true;
		}

		if (maxSecond <= 1.5f)
		{
			maxSecond = 1.5f;
		}
	}

	if (isSpeedUp)
	{
		if (textSpeedUpCnt == 0)
		{
			arrow.tex.x = 0.0f;
		}
		else if (textSpeedUpCnt % 5 == 0 && textSpeedUpCnt != 0)
		{
			arrow.tex.x += 46.0f;
			if (46.0f * 3.0f < arrow.tex.x)
			{
				arrow.tex.x = 0.0f;
			}
		}
		
		if (textSpeedUpCnt <= 60)
		{
			alpha = easing::OutExp(static_cast<float>(textSpeedUpCnt), 60.0f, 1.0f, 0.0f);
			speedUp.pos.y = easing::OutExp(static_cast<float>(textSpeedUpCnt), 60.0f, SPEED_UP_POS_Y, METER_POS_Y);
		}
		if (SPEED_UP_CNT_MAX * 60 <= textSpeedUpCnt++)
		{
			alpha = 0.0f;
			speedUp.pos.y = METER_POS_Y;
			isSpeedUp = false;
			arrow.tex.x = 0.0f;
			textSpeedUpCnt = 0;
		}
	}

#ifdef USE_IMGUI
//	ImGui::Begin(u8"debug");
//	ImGui::Text(u8"秒数：%f", second);
//	ImGui::Text(u8"speedUpCount : %d", speedUpCount);
//	ImGui::Text(u8"maxSecond : %f", maxSecond);
//	ImGui::Text(u8"経過時間　%d : %d", minTime, secondTime);
////	ImGui::SliderFloat("tenpos.x",&debugTen.x,0,1920);
//	ImGui::SliderFloat("pos.x",&readyPosX, 0, 1920);
//	ImGui::SliderFloat("pos.y", &readyPosY, 0, 1080);
	//	ImGui::SliderFloat("oneos.y", &debugone.y, 0, 1080);
//	ImGui::SliderFloat("renpos.x", &debugRen.x, 0, 1920);
//	ImGui::SliderFloat("renpos.y", &debugRen.y, 0, 1080);
//	if (isTimerStop) ImGui::Text(u8"isTimerStop：true");
//	else ImGui::Text(u8"isTimerStop：false");
//	ImGui::End();

//	if (_playerNum == 0)
//	{
//		ImGui::Begin(u8"totalScore");
//		ImGui::Text("score : %d", totalScore);
//		ImGui::End();
//	}
//	else
//	{
//		ImGui::Begin(u8"totalScore");
//		ImGui::Text("score : %d", totalScore);
//		ImGui::End();
//	}
//	ImGui::Begin("pos");
//	ImGui::SliderFloat("pos.x", &tryPosX, 0, 1920);
//	ImGui::SliderFloat("pos.y", &tryPosY, 0, 1080);
//	ImGui::End();
#endif

	totalTime++;

	ChainDisplayUpdate();
}

void UI::Draw(int _playerNum)
{
	DirectX::XMFLOAT2 normalCenter(0.0f, 0.0f);
	DirectX::XMFLOAT4 normalColor(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT2 tex(0, 696);
	DirectX::XMFLOAT2 size(36, 60);

	//次出てくるブロックの色のやつ
	SetDrawBlendMode(BLEND_MODE::ALPHA);
	sprUI->Begin();
	if (!sceneGame.isTwoPlayer)
	{
		for (int i = 0; i < 6; i++)
		{
			sprUI->Draw(DirectX::XMFLOAT2(block[i].pos.x, block[i].pos.y + GAME_MASTER->blockManager[_playerNum].shakePos.y), block[rowColor[i]].size, block[rowColor[i]].tex, block[rowColor[i]].size,
				DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else
	{
		switch (_playerNum)
		{
		case 0:
			for (int i = 0; i < 6; i++)
			{
				sprUI->Draw(block[i].pos, block[rowColor[i]].size, block[rowColor[i]].tex, block[rowColor[i]].size,
					DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			break;
		case 1:
			for (int i = 0; i < 6; i++)
			{
				sprUI->Draw(block[i].pos, block[rowColor[i]].size, block[rowColor[i]].tex, block[rowColor[i]].size,
					DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			break;
		}
	}

	// Meter Back
	sprUI->Draw(meterBack.pos, meterBack.size, meterBack.tex, meterBack.size,
		DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// Meter
	sprUI->Draw(DirectX::XMFLOAT2(meter.pos.x, posY), DirectX::XMFLOAT2(meter.size.x, tSizeY),
		DirectX::XMFLOAT2(meter.tex.x, tPosY), DirectX::XMFLOAT2(meter.size.x, tSizeY),
		DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// speedUP!
	if (isSpeedUp)
	{
		sprUI->Draw(speedUp.pos, speedUp.size, speedUp.tex, speedUp.size,
			DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));
	}

	// arrow
	sprUI->Draw(arrow.pos, arrow.size, arrow.tex, arrow.size,
		DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ScoreBoard
	int hundredThousand = totalScore % 1000000 / 100000;
	int tenThousand = totalScore % 100000 / 10000;
	int thousand = totalScore % 10000 / 1000;
	int hundred = totalScore % 1000 / 100;
	int ten = totalScore % 100 / 10;
	int one = totalScore % 10 / 1;
	if (!sceneGame.isTwoPlayer)
	{
		sprUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, normalColor);

		// Score

		// 十万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundredThousand/*ここに桁入れる*/, tex.y), size, normalCenter, 0.0f, normalColor);
		// 万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * tenThousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 千
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * thousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 百
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundred, tex.y), size, normalCenter, 0.0f, normalColor);
		// 十
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);
		// 一
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);
	}
	else
	{
		TwoPlayDrawScore(_playerNum);
	}

	// ChainCount
	if (!sceneGame.isTwoPlayer)
	{
		if (isDisplayChain)
		{
			if (!sceneGame.isGameOver_two && !sceneGame.isGameOver_one)
			{
				if (GAME_MASTER->blockManager[_playerNum].chainCount != 0)
				{
					lastChainCount = GAME_MASTER->blockManager[_playerNum].chainCount;
				}
				tex = DirectX::XMFLOAT2(0, 816);
				normalColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, chainDisplayAlpha);
				if (GAME_MASTER->blockManager[_playerNum].chainCount >= 10)
				{

					ten = lastChainCount % 100 / 10;
					one = lastChainCount % 10 / 1;
					// 十
					sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);//1
					// 一
					sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);//2
					//連！
					sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 162 + ADJUST + ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);
				}
				else
				{
					// 一
					sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * lastChainCount, tex.y), size, normalCenter, 0.0f, normalColor);//1
					// 連！
					sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);//2
				}
			}
		}
		//-------------------
		// GameResultの描画
		//-------------------

		if (GAME_MASTER->blockManager[0].resultDisplayAlpha != 0.0f)
		{
			// Result
			sprUI->Draw(GAME_MASTER->blockManager[0].resultDisplayPosX, GAME_MASTER->blockManager[0].resultDisplayPosY - 129, 680, 150, 0, 2667, 680, 150, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].resultDisplayAlpha);
			// score
			sprUI->Draw(669, GAME_MASTER->blockManager[0].resultDisplayPosY2 -129, 160, 64, 0, 2667 + 150, 160, 64, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].resultDisplayAlpha);
		}
		if (GAME_MASTER->blockManager[0].scoreDisplayAlpha != 0.0f)
		{
			hundredThousand = totalScore % 1000000 / 100000;
			tenThousand = totalScore % 100000 / 10000;
			thousand = totalScore % 10000 / 1000;
			hundred = totalScore % 1000 / 100;
			ten = totalScore % 100 / 10;
			one = totalScore % 10 / 1;
			sprUI->Draw(DirectX::XMFLOAT2(744 + 0 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * hundredThousand,816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));
			sprUI->Draw(DirectX::XMFLOAT2(744 + 1 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * tenThousand, 816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));
			sprUI->Draw(DirectX::XMFLOAT2(744 + 2 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * thousand, 816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));
			sprUI->Draw(DirectX::XMFLOAT2(744 + 3 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * hundred, 816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));
			sprUI->Draw(DirectX::XMFLOAT2(744 + 4 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * ten, 816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));
			sprUI->Draw(DirectX::XMFLOAT2(744 + 5 * 72, GAME_MASTER->blockManager[0].scoreDisplayPosY - 129), DirectX::XMFLOAT2(36 * 2, 60 * 2), DirectX::XMFLOAT2(tex.x + 36 * one, 816), size, DirectX::XMFLOAT2(0, 0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].scoreDisplayAlpha));

		}

		if (GAME_MASTER->blockManager[0].gameResultState == 5/*NEXT_BEHAVIOR*/)
		{
			if (GAME_MASTER->blockManager[0].isResultSelectLeft)
			{
				sprUI->Draw(618,628/*tryPosX, tryPosY*/,				312, 114, 0,	3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				sprUI->Draw(990,628/*tryPosX + 312, tryPosY*/,			312, 114, 312, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				sprUI->Draw(618,628/*tryPosX, tryPosY*/, 312, 114, 312, 3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				sprUI->Draw(990,628/*tryPosX, tryPosY*/, 312, 114, 0, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	else
	{
		if (!sceneGame.isGameOver_two || !sceneGame.isGameOver_one)
		{
			TwoPlayDrawChainCount(_playerNum);
		}

		if (GAME_MASTER->blockManager[0].resultDisplayAlpha != 0.0f)
		{
			if (sceneGame.isGameOver_one)
			{
				if (_playerNum == 0)
				{
					switch (isWinLoseUp)
					{
					case true:
						GAME_MASTER->ui[0].winLoseAlpha += 0.01f;
						if (GAME_MASTER->ui[0].winLoseAlpha >= 1.5f)
						{
							isWinLoseUp = false;
						}
						break;
					case false:
						GAME_MASTER->ui[0].winLoseAlpha -= 0.01f;
						if (GAME_MASTER->ui[0].winLoseAlpha <= 1.0f)
						{
							isWinLoseUp = true;
						}
						break;
					}
				}
				switch (_playerNum)
				{
				case 0:
					//Win
					sprUI->Draw(1251, GAME_MASTER->blockManager[0].resultDisplayPosY, 618, 159, 0, 2348, 618, 159, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].resultDisplayAlpha);
					SetDrawBlendMode(BLEND_MODE::ADD);
					sprUI->Draw(1251, GAME_MASTER->blockManager[0].resultDisplayPosY, 618, 159, 0, 2348, 618, 159, 0, 0, 0.0, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha);
					SetDrawBlendMode(BLEND_MODE::ALPHA);
					//Lose
					sprUI->Draw(39, GAME_MASTER->blockManager[1].resultDisplayPosY, 618, 159, 0, 2348 + 159, 618, 159, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[1].resultDisplayAlpha);
					SetDrawBlendMode(BLEND_MODE::ADD);
					sprUI->Draw(39, GAME_MASTER->blockManager[1].resultDisplayPosY, 618, 159, 0, 2348 + 159, 618, 159, 0, 0, 0.0, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha);
					SetDrawBlendMode(BLEND_MODE::ALPHA);
					break;
				case 1:
					break;
				}
			}
			else if (sceneGame.isGameOver_two)
			{
				if (_playerNum == 0)
				{
					switch (isWinLoseUp)
					{
					case true:
						GAME_MASTER->ui[0].winLoseAlpha += 0.01f;
						if (GAME_MASTER->ui[0].winLoseAlpha >= 1.5f)
						{
							isWinLoseUp = false;
						}
						break;
					case false:
						GAME_MASTER->ui[0].winLoseAlpha -= 0.01f;
						if (GAME_MASTER->ui[0].winLoseAlpha <= 1.0f)
						{
							isWinLoseUp = true;
						}
						break;
					}
				}
				switch (_playerNum)
				{
				case 0:
					//Win
					sprUI->Draw(39, GAME_MASTER->blockManager[0].resultDisplayPosY, 618, 159, 0, 2348, 618, 159, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[0].resultDisplayAlpha);
					SetDrawBlendMode(BLEND_MODE::ADD);
					sprUI->Draw(39, GAME_MASTER->blockManager[0].resultDisplayPosY, 618, 159, 0, 2348, 618, 159, 0, 0, 0.0, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha);
					SetDrawBlendMode(BLEND_MODE::ALPHA);
					break;
				case 1:
					//Lose
					sprUI->Draw(1251, GAME_MASTER->blockManager[1].resultDisplayPosY, 618, 159, 0, 2348 + 159, 618, 159, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, GAME_MASTER->blockManager[1].resultDisplayAlpha);
					SetDrawBlendMode(BLEND_MODE::ADD);
					sprUI->Draw(1251, GAME_MASTER->blockManager[1].resultDisplayPosY, 618, 159, 0, 2348 + 159, 618, 159, 0, 0, 0.0, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha, GAME_MASTER->ui[0].winLoseAlpha);
					SetDrawBlendMode(BLEND_MODE::ALPHA);
					break;
				}
			}
		}
		if (GAME_MASTER->blockManager[0].gameResultState == 5/*NEXT_BEHAVIOR*/ || GAME_MASTER->blockManager[1].gameResultState == 5)
		{
			if (sceneGame.isGameOver_two)
			{
				if (GAME_MASTER->blockManager[0].isResultSelectLeft)
				{
					sprUI->Draw(618 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 0, 3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					sprUI->Draw(990 - 591, 628/*tryPosX + 312, tryPosY*/, 312, 114, 312, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					sprUI->Draw(618 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 312, 3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					sprUI->Draw(990 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 0, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			else if(sceneGame.isGameOver_one)
			{
				if (GAME_MASTER->blockManager[1].isResultSelectLeft)
				{
					sprUI->Draw(618 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 0, 3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					sprUI->Draw(990 - 591, 628/*tryPosX + 312, tryPosY*/, 312, 114, 312, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					sprUI->Draw(618 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 312, 3136, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					sprUI->Draw(990 - 591, 628/*tryPosX, tryPosY*/, 312, 114, 0, 3136 + 114, 312, 114, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
	}

	// Ready
	if (sceneGame.isStartInit)
	{
		if (GAME_MASTER->ui[0].readyState < 1)
		{
			sprUI->Draw(GAME_MASTER->ui[0].readyPosX, GAME_MASTER->ui[0].readyPosY, 576, 150, 0, 3364, 576, 150, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, GAME_MASTER->ui[0].readyAlpha);
		}
		else
		{
			if(!sceneGame.isGameOver_one && !sceneGame.isGameOver_two)
			sprUI->Draw(GAME_MASTER->ui[0].readyPosX, GAME_MASTER->ui[0].readyPosY, 576, 158, 0, 3364 + 158, 576, 158, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, GAME_MASTER->ui[0].readyAlpha);
		}
	}

	sprUI->End();
	SetDrawBlendMode(BLEND_MODE::ALPHA);
}

void UI::Uninit()
{

}


int UI::calculateScore(int _eraseNum, int _playerNum)
{
	float nowPlusScore;
//	int baseScore;

	int chainCount = GAME_MASTER->blockManager[_playerNum].chainCount;

	if (chainCount >= 5)
	{
		chainCount = 5;
	}

//	switch (BLOCK_MASTER->blockManager[0].chainCount)
//	{
//	case 1:
//		baseScore = 10;
//		break;
//	case 2:
//		baseScore = 20;
//		break;
//	case 3:
//		baseScore = 40;
//		break;
//	case 4:
//		baseScore = 60;
//		break;
//	case 5:
//		baseScore = 80;
//		break;
//	default:
//		baseScore = 120;
//		break;
//	}
//
	if (_eraseNum - 2 == 0)
	{
		nowPlusScore = static_cast<float>(baseScore_tbl[chainCount]);
	}
	else
	{
		nowPlusScore = baseScore_tbl[chainCount] * (_eraseNum - 2) * 1.5f;
	}
//	assert(nowPlusScore <= 10000);
	return static_cast<int>(nowPlusScore);
}

void UI::SetChainDisplayInit(DirectX::XMFLOAT2 _pos)
{
	chainPos = _pos;
	chainPos.y += 57;
	isDisplayChain = true;
	chainDisplayTimer = 0;
	chainDisplayAlpha = 0.0f;
	chainFirstPosY = chainPos.y;
}

void UI::ChainDisplayUpdate()
{
	if (!isDisplayChain) return;

	if (chainDisplayTimer < 60)
	{
		if (chainDisplayTimer <= 10)
		{
			chainDisplayAlpha += 0.1f;
		}
		chainPos.y = easing::OutExp(static_cast<float>(chainDisplayTimer), 60.0f, chainFirstPosY - 57.0f, chainFirstPosY);
		++chainDisplayTimer;
	}
	else
	{
		chainDisplayAlpha -= 0.1f;
		if (chainDisplayAlpha <= 0.0f)
		{
			chainDisplayAlpha = 0.0f;
		}
		++chainDisplayTimer;
	}

	if (chainDisplayTimer >= 120)
	{
		chainDisplayTimer = 0;
		isDisplayChain = false;
	}
}

void UI::TwoPlayDrawChainCount(int _playerNum)
{
	DirectX::XMFLOAT2 size(36, 60);
	DirectX::XMFLOAT2 tex(0, 816);
	DirectX::XMFLOAT4 normalColor(1.0f, 1.0f, 1.0f, chainDisplayAlpha);
	DirectX::XMFLOAT2 normalCenter(0.0f, 0.0f);
	if (isDisplayChain)
	{
		if (sceneGame.isGameOver_one || sceneGame.isGameOver_two)return;
		switch (_playerNum)
		{
		case 0:
			if (GAME_MASTER->blockManager[_playerNum].chainCount != 0)
			{
				lastChainCount = GAME_MASTER->blockManager[_playerNum].chainCount;
			}

			if (GAME_MASTER->blockManager[_playerNum].chainCount >= 10)
			{

				int ten = lastChainCount % 10 / 1;
				int one = lastChainCount - ten * 10;
				// 十
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + TWO_PLAY_ONE_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);//1
				 // 一
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + TWO_PLAY_ONE_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);//2
				  //連！
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 162 + ADJUST + TWO_PLAY_ONE_ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);
			}
			// 一
			sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + TWO_PLAY_ONE_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * lastChainCount, tex.y), size, normalCenter, 0.0f, normalColor);//1
			// 連！
			sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + TWO_PLAY_ONE_ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);//2
			break;
		case 1:
			if (GAME_MASTER->blockManager[_playerNum].chainCount != 0)
			{
				lastChainCount = GAME_MASTER->blockManager[_playerNum].chainCount;
			}

			if (GAME_MASTER->blockManager[_playerNum].chainCount >= 10)
			{

				int ten = lastChainCount % 10 / 1;
				int one = lastChainCount - ten * 10;
				// 十
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + TWO_PLAY_TWO_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);//1
				 // 一
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + TWO_PLAY_TWO_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);//2
				//連！
				sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 162 + ADJUST + TWO_PLAY_TWO_ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);
			}
			// 一
			sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + ADJUST + TWO_PLAY_TWO_ORIJIN_X, chainPos.y), DirectX::XMFLOAT2(size.x * 2, size.y * 2), DirectX::XMFLOAT2(tex.x + 36 * lastChainCount, tex.y), size, normalCenter, 0.0f, normalColor);//1
			// 連！
			sprUI->Draw(DirectX::XMFLOAT2(chainPos.x + 81 + ADJUST + TWO_PLAY_TWO_ORIJIN_X, chainPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(tex.x, tex.y + 60), DirectX::XMFLOAT2(66, 60), normalCenter, 0.0f, normalColor);//2

			break;
		}
	}
}

void UI::TwoPlayDrawScore(int _playerNum)
{
	DirectX::XMFLOAT4 normalColor(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT2 normalCenter(0.0f, 0.0f);
	DirectX::XMFLOAT2 tex(0, 696);
	DirectX::XMFLOAT2 size(36, 60);
	int hundredThousand = totalScore % 1000000 / 100000;
	int tenThousand = totalScore % 100000 / 10000;
	int thousand = totalScore % 10000 / 1000;
	int hundred = totalScore % 1000 / 100;
	int ten = totalScore % 100 / 10;
	int one = totalScore % 10 / 1;
	switch (_playerNum)
	{
	case 0:
		sprUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, normalColor);

		// Score

		hundredThousand = totalScore % 1000000 / 100000;
		tenThousand = totalScore % 100000 / 10000;
		thousand = totalScore % 10000 / 1000;
		hundred = totalScore % 1000 / 100;
		ten = totalScore % 100 / 10;
		one = totalScore % 10 / 1;
		// 十万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundredThousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * tenThousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 千
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * thousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 百
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundred, tex.y), size, normalCenter, 0.0f, normalColor);
		// 十
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);
		// 一
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);

		break;
	case 1:
		sprUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, normalColor);

		// Score

		hundredThousand = totalScore % 1000000 / 100000;
		tenThousand = totalScore % 100000 / 10000;
		thousand = totalScore % 10000 / 1000;
		hundred = totalScore % 1000 / 100;
		ten = totalScore % 100 / 10;
		one = totalScore % 10 / 1;
		tex.y += 60.0f;
		// 十万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundredThousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 万
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * tenThousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 千
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * thousand, tex.y), size, normalCenter, 0.0f, normalColor);
		// 百
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * hundred, tex.y), size, normalCenter, 0.0f, normalColor);
		// 十
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * ten, tex.y), size, normalCenter, 0.0f, normalColor);
		// 一
		sprUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(tex.x + 36 * one, tex.y), size, normalCenter, 0.0f, normalColor);

		break;
	}
}