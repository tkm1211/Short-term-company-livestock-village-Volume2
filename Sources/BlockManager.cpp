//#include <GamePad.h>
#include <input_device.h>
#include <algorithm>
#include <Blender.h>
#include <assert.h>
#include "easing.h"
#include "BlockManager.h"
#include "GameMaster.h"
#include "SceneManager.h"
#include "Player.h"
#include "Effect.h"
#include "Sound.h"
#include "UI.h"

#define END_MAX_PAR 40
#define END_MIN_PAR 30

#define MAX_PAR 35
#define MIN_PAR 30


void BlockManager::Init(int _playerNum)
{
	sprBlock = std::make_unique<SpriteBatch>(L"Data/block.png",BLOCK_MAX,DirectX::XMFLOAT2(114,114));

	// TODO : レベルセレクトからデータをもらう
	if (_playerNum == GameMode::ONE_PLAYER)
	{
		colorMax = sceneSelect.colorNum;
	}
	else if (_playerNum == GameMode::TWO_PLAYER)
	{
		colorMax = sceneSelect.colorNum2;
	}

	int lastRnd = rand() % colorMax;
	GenerateBlock(0, 5, lastRnd);

	for (int i = 1; i < 6; i++)
	{
		int rnd = rand() % colorMax;
		while (rnd == lastRnd)
		{
			rnd = rand() % colorMax;
		}
		lastRnd = rnd;
		GenerateBlock(i, 5, rnd);
	}

//	GenerateBlock(0, 5, 3);
//	GenerateBlock(1, 5, 2);
//	GenerateBlock(2, 5, 1);
//	GenerateBlock(3, 5, 0);
//	GenerateBlock(4, 5, 3);
//	GenerateBlock(5, 5, 2);

	SetStartBlock();

	for (auto& it : block)
	{
		it.Init();
	}

	pushUpCnt = 0;
	pushUp = false;
	pushUpT = false;
	isChain = false;
	isBreak = false;
	isChainNow = false;
	sleep[0] = 0;
	sleep[1] = 0;
	sleep[2] = 0;

	isPopRowLine = false;
	PopRowLine();

	isTimePushUpBoard = false;

	isLastChain = false;

	desFunction.dummyBlock.color = -1;
	desFunction.dummyBlock.row = -1;
	desFunction.dummyBlock.column = -1;
	desFunction.dummyBlock.Init();

	for (int i = 0; i < COLUMN_MAX; i++)
	{
		for (int j = 0; j < ROW_MAX; j++)
		{
			desFunction.sortBlock[i][j] = desFunction.dummyBlock;
		}
	}

	for (int i = 0; i < ROW_MAX; i++)
	{
		desFunction._block[i] = desFunction.dummyBlock;
	}

	desFunction._num = 0;
	desFunction.isData = false;

	for (int i = 0; i < 54; i++)
	{
		desFunction.allElement[i] = -1;
	}
	desFunction.allElementCnt = 0;

	chainCount = 0;
	eraseCount = 0;

	// Relation of Obstacle
	obstacleCount = 0;
	obstacleFallWaitTime = 0;
	fallObstacleCount = 0;
	obstacleFallWaiting = false;
	nowObstacleFall = false;

	// GameResult Variables
	gameResultState = START;
	shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
	shakePos2 = DirectX::XMFLOAT2(0.0f, 0.0f);
	shakeTimer = 0;
	eraseBlockCount = 0;
	isInitBreak = false;
	resultDisplayAlpha = 0.0f;
	resultDisplayTimer = 0;
	resultDisplayPosX = 615;
	resultDisplayPosY = 288;
	resultDisplayPosY2 = 471;
	resultDisplayPosXFirst = 615;
	resultDisplayPosYFirst = 378;
	scoreDisplayAlpha = 0.0f;
	scoreDisplayPosY = 546.0f;
	resultDisplayMultiAlpha = 0.0f;
	resultCount = 0;
	resultCount2 = 0;
	resultCount3 = 0;
	isResultSelectLeft = true;
}

void BlockManager::Update(int _playerNum)
{
	switch(sceneGame.isTwoPlayer)
	{
	case false:
		if (!sceneGame.isGameOver_one)									// 1人通常ゲーム
		{
			ProcessOfGame(_playerNum);
		}
		else															// 1人リザルト
		{
			ProcessOfGameResultOnePlayer(_playerNum);
		}
		break;

	case true:
		if (!sceneGame.isGameOver_one && !sceneGame.isGameOver_two)	// 2人通常ゲーム
		{
			ProcessOfGame(_playerNum);
		}
		else														// 2人リザルト
		{
			ProcessOfGameResultTwoPlayer(_playerNum);
		}
		break;
	}

}

void BlockManager::Draw(int _playerNum)
{
	sprBlock->Begin();

	if (!sceneGame.isTwoPlayer)
	{
		for (auto &it : block)
		{
			if (9 <= it.column)
			{
				continue;
			}
			sprBlock->Draw(it.pos.x + ADJUST + ORIJIN_X, it.pos.y + ADJUST + ORIJIN_Y + shakePos.y, static_cast<float>(it.SIZE_X), static_cast<float>(it.SIZE_Y), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.color);
		}
	}
	else
	{
		TwoPlayDraw(_playerNum);
	}
	sprBlock->End();
}

void BlockManager::Uninit()
{
	block.clear();
}


//-----------------------------------------------------------------------

//	各種関数

//-----------------------------------------------------------------------
// ゲーム処理系
void BlockManager::ProcessOfGame(int _playerNum)
{
	// 仮 ブロックを一段生成
	/*if (InputTrigger(XINPUT_RIGHT_THUMB))
	{
		PushUpBoard(_playerNum);
	}*/
	if (pushUp)
	{
		GAME_MASTER->player[_playerNum].pos.y -= 114.0f / 6.0f;
		GAME_MASTER->player[_playerNum].canMove = false;
		MoveBoard(_playerNum);
	}

	for (size_t i = 0; i < block.size(); i++)
	{
		block[i].Update(_playerNum);
	}

	if (isBreak && !isChain)
	{
		int count = 0;
		for (auto& it : block)
		{
			if (it.isFall)
			{
				count++;
			}
		}
		if (count == 0)
		{
			isBreak = false;
			isChain = true;
		}
	}

	if (isChain)
	{
		GAME_MASTER->ui[_playerNum].isTimerStop = true;
		if (30 <= sleep[0]++)
		{
			RagisterChainBlock(_playerNum);
			if (eraseCount != 0)
			{
				GAME_MASTER->ui[_playerNum].totalScore += GAME_MASTER->ui[_playerNum].calculateScore(eraseCount, _playerNum);
//				assert(GAME_MASTER->ui[_playerNum].totalScore <= 100000 && "totalScore over the handredMilion");
				eraseCount = 0;
			}

			sleep[0] = 0;
			if (!isChainNow)
			{
				//GAME_MASTER->ui[_playerNum].isTimerStop = false;
				isChain = false;
				if (!JudgeIsFallObstacle())
					PushUpBoard(_playerNum);
			}
			else
			{
				chainCount++;
				if (sceneGame.isTwoPlayer)
				{
					AttackOfObstacleRelation(_playerNum);
				}
			}
		}
	}

	if (isLastChain)
	{
		int count = 0;
		for (auto& it : block)
		{
			if (it.isFall)
			{
				count++;
			}
		}
		if (count == 0)
		{
			GAME_MASTER->ui[_playerNum].isTimerStop = true;
			if (30 <= sleep[2]++)
			{
				RagisterChainBlock(_playerNum);
				if (eraseCount != 0)
				{
					GAME_MASTER->ui[_playerNum].totalScore += GAME_MASTER->ui[_playerNum].calculateScore(eraseCount, _playerNum);
//					assert(GAME_MASTER->ui[_playerNum].totalScore <= 100000 && "totalScore over the handredMilion");
					eraseCount = 0;
				}

				sleep[2] = 0;
				if (!isChainNow)
				{
					GAME_MASTER->ui[_playerNum].isTimerStop = false;
					GAME_MASTER->ui[_playerNum].isGageMax = false;
					if (_playerNum == 1 && (int)GAME_MASTER->ui[_playerNum].second <= 0)
					{ 
						GAME_MASTER->ui[_playerNum].meter.tex.x = 0.0f;
					}
					GAME_MASTER->ui[_playerNum].meter.tex.y = 936.0f;
					isLastChain = false;
					chainCount = 0;
					//PushUpBoard();
				}
				else
				{
					chainCount++;
					if (sceneGame.isTwoPlayer)
					{
						AttackOfObstacleRelation(_playerNum);
					}
				}
			}
		}
	}

	if (isPopRowLine)
	{
		PopRowLine();
		isPopRowLine = false;
	}

	if (isTimePushUpBoard && !pushUp)
	{
		if (!JudgeIsFallObstacle())
		{
			PushUpBoard(_playerNum);
			pushUpT = true;
		}
		isTimePushUpBoard = false;
	}

	JudgeUpToOnBlock(_playerNum);

	// お邪魔ブロックを受ける関数
	ReceiveOfObstacleRelation(_playerNum);

#ifdef USE_IMGUI
	ImGui::Begin(u8"おじゃま");
	switch (_playerNum)
	{
	case 0:
		ImGui::Text("obstacleCount1 : %d", obstacleCount);
		ImGui::Text("obstacleFallWaitTime1 : %d", obstacleFallWaitTime);
		ImGui::Text("obstacleFallWaiting1 : %d", obstacleFallWaiting);
		break;
	case 1:
		ImGui::Text("obstacleCount2 : %d", obstacleCount);
		ImGui::Text("obstacleFallWaitTime2 : %d", obstacleFallWaitTime);
		ImGui::Text("obstacleFallWaiting2 : %d", obstacleFallWaiting);
		break;
	}
	//	ImGui::Text("isChain : %d", isChain);
	//	ImGui::Text("isLastChain : %d", isLastChain);
	//	ImGui::Text("isChainNow : %d", isChainNow);
	//	ImGui::Text("pushUp : %d", pushUp);
	ImGui::End();
#endif
	//	switch (_playerNum)
	//	{
	//	case 0:
	//		ImGui::Begin("count");
	//		ImGui::Text("1");
	//		ImGui::Text("nowObstacleFall : %d", nowObstacleFall);
	//		ImGui::Text("fallObstacleCount : %d", fallObstacleCount);
	//		ImGui::End();
	//		break;
	//	case 1:
	//		ImGui::Begin("count");
	//		ImGui::Text("2");
	//		ImGui::Text("nowObstacleFall : %d", nowObstacleFall);
	//		ImGui::Text("fallObstacleCount : %d", fallObstacleCount);
	//		ImGui::End();
	//		break;
	//	}
//	ImGui::Begin(u8"pos");
//	switch (_playerNum)
//	{
//	case 0:
//		ImGui::SliderFloat("posX", &resultDisplayPosXFirst, 0, 1920);
//		ImGui::SliderFloat("posY", &resultDisplayPosYFirst, 0, 1080);
//		break;
//	case 1:
//		ImGui::SliderFloat("posX2", &resultDisplayPosXFirst2, 0, 1920);
//		ImGui::SliderFloat("posY2", &resultDisplayPosYFirst2, 0, 1080);
//		break;
//	}
//	ImGui::End();
}

void BlockManager::ProcessOfGameResultOnePlayer(int _playerNum)
{
	switch (gameResultState)
	{
	case GAME_RESULT_SINGLE::START:
		gameResultState++;
//		break;
	case SHAKE:
		if (shakeTimer % 4 >= 2)
		{
			shakePos.y = -5.0;
		}
		else
		{
			shakePos.y = 5.0f;
		}
		if (++shakeTimer >= 20)
		{
			gameResultState++;
			shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
		}
		break;
	case GAME_RESULT_SINGLE::BREAK_BLOCK:
		if (!isInitBreak)
		{
			breakCount = 0;
			for (int i = 0; i < 10; i++) //column + 上にいった一つ
			{
				for (int j = 0; j < 6; j++)
				{
					fixBlockData[i][j] = -1;
				}
			}
			for (size_t i = 0; i < block.size(); i++)
			{
				fixBlockData[block[i].column + 1][block[i].row] = block[i].column + 1;
				assert(gameResultState == 2 && "gameResultState error");
			}
			isInitBreak = true;
		}
		else
		{
			if (++breakCount % 20 == 0)			// 1秒に一回上から壊す
			{
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 6; j++)
					{
						if (fixBlockData[i][j] == eraseBlockCount)
						{
							for (size_t k = 0; k < block.size(); k++)
							{
								if (block[k].row == j && block[k].column + 1 == i)
								{
									if (block[k].color != 7)
									{
										GAME_MASTER->effect[_playerNum].GenerateParticle(block[k].row, block[k].column, block[k].color);
									}
									block.erase(block.begin() + k);
								}
							}
						}
					}
				}
				
				eraseBlockCount++;
			}
			if (eraseBlockCount == 9)
			{
				block.clear();
				gameResultState++;
			}
		}
		break;
	case DISPLAY_RESULT:
		if (resultDisplayTimer <= 10)
		{
			resultDisplayAlpha += 0.1f;
		}
		if (resultDisplayAlpha == 0.1f)
		{
			pAudio->Play(SOUND->seHandle[SOUND->RESULT]);
		}
		resultDisplayPosY  = easing::OutExp((float)resultDisplayTimer, 60, 378 - 57, 378);
		resultDisplayPosY2 = easing::OutExp((float)resultDisplayTimer, 60, 471 , 471 + 57);
		++resultDisplayTimer;

		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
		}
		break;
	case GAME_RESULT_SINGLE::DISPLAY_SCORE:
		if (resultDisplayTimer <= 10)
		{
			scoreDisplayAlpha += 0.1f;
		}
		if (scoreDisplayAlpha == 0.1f)
		{
			pAudio->Play(SOUND->seHandle[SOUND->RESULT]);
		}

		scoreDisplayPosY = easing::OutExp((float)resultDisplayTimer, 60, 546, 546 + 57);

		++resultDisplayTimer;
		if (resultDisplayTimer >= 120)
		{
			gameResultState++;
			resultDisplayTimer = 0;
			pAudio->Play(SOUND->seHandle[SOUND->RESULT]);
		}
		break;
	case GAME_RESULT_SINGLE::NEXT_BEHAVIOR:

		static int count;
		static int count2;
		static int count3;
		if (pad[0].bLEFTt || pad[0].sX < 0)
		{
			if (++count == 1)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				isResultSelectLeft = true;
			}
		}
		else
		{
			count = 0;
		}
		
		if (pad[0].bRIGHTt || 0 < pad[0].sX)
		{
			if (++count2 == 1)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				isResultSelectLeft = false;
			}
		}
		else
		{
			count2 = 0;
		}

		if (pad[0].bAt)
		{
			if (++count3 == 1)
			{
				if (isResultSelectLeft)
				{
					//リトライ
					pAudio->Play(SOUND->seHandle[SOUND->OK]);
					sceneGame.goGameInit = true;
					sceneGame.goTitle = false;
					sceneGame.isStartInit = true;
					sceneGame.startTimer = 0;
				}
				else
				{
					//タイトル
					pAudio->Play(SOUND->seHandle[SOUND->OK]);
					sceneGame.goTitle = true;
					sceneGame.goGameInit = false;
					sceneGame.isStartInit = true;
					sceneGame.startTimer = 0;
				}
			}
		}
		else
		{
			count3 = 0;
		}

//		if (InputTrigger(XINPUT_A, _playerNum))
//		{
//			sceneGame.goTitle = true;
//			sceneGame.goGameInit = false;
//		}
//		else if (InputTrigger(XINPUT_B, _playerNum))
//		{
//			sceneGame.goTitle = false;
//			sceneGame.goGameInit = true;
//		}
		break;
	default:
		break;
	}
}

void BlockManager::ProcessOfGameResultTwoPlayer(int _playerNum)
{
	switch (gameResultState)
	{
	case MULTI_START:
		gameResultState++;
		//		break;
	case GAME_RESULT_MALTI::MULTI_SHAKE:
		if (sceneGame.isGameOver_one)
		{
			if (shakeTimer % 4 >= 2)
			{
				shakePos.y = -5.0;
			}
			else
			{
				shakePos.y = 5.0f;
			}
			if (++shakeTimer >= 20)
			{
				gameResultState++;
				shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
			}

		}
		else if (sceneGame.isGameOver_two)
		{
			if (shakeTimer % 4 >= 2)
			{
				shakePos2.y = -5.0;
			}
			else
			{
				shakePos2.y = 5.0f;
			}
			if (++shakeTimer >= 20)
			{
				gameResultState++;
				shakePos2 = DirectX::XMFLOAT2(0.0f, 0.0f);
			}

		}
		break;
	case GAME_RESULT_MALTI::MULTI_RAGISTER_BREAK_BLOCK:
		if (sceneGame.isGameOver_one)
		{
			if (_playerNum == 0)
			{
				if (!isInitBreak)
				{
					breakCount = 0;
					for (int i = 0; i < 10; i++) //column + 上にいった一つ
					{
						for (int j = 0; j < 6; j++)
						{
							fixBlockData[i][j] = -1;
						}
					}
					for (size_t i = 0; i < block.size(); i++)
					{
						fixBlockData[block[i].column + 1][block[i].row] = block[i].column + 1;
						assert(gameResultState == 2 && "gameResultState error");
					}
					isInitBreak = true;
				}
				gameResultState++;
			}
			else
			{
				gameResultState++;
				break;	//WIN側は何もしない
			}
		}
		else if (sceneGame.isGameOver_two)
		{
			if (_playerNum == 0)
			{
				gameResultState++;
				break;	//WIN側は何もしない
			}
			else
			{
				if (!isInitBreak)
				{
					breakCount = 0;
					for (int i = 0; i < 10; i++) //column + 上にいった一つ
					{
						for (int j = 0; j < 6; j++)
						{
							fixBlockData[i][j] = -1;
						}
					}
					for (size_t i = 0; i < block.size(); i++)
					{
						fixBlockData[block[i].column + 1][block[i].row] = block[i].column + 1;
						assert(gameResultState == 2 && "gameResultState error");
					}
					isInitBreak = true;
					gameResultState++;
				}
			}
		}

		break;
	case GAME_RESULT_MALTI::MULTI_BREAK_BOCK:
		if (sceneGame.isGameOver_one)
		{
			if (_playerNum == 0)	//Lose
			{
				if (++breakCount % 20 == 0)			// 1秒に一回上から壊す
				{
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (fixBlockData[i][j] == eraseBlockCount)
							{
								for (size_t k = 0; k < block.size(); k++)
								{
									if (block[k].row == j && block[k].column + 1 == i)
									{
										if (block[k].color != 7)
										{
											GAME_MASTER->effect[_playerNum].GenerateParticle(block[k].row, block[k].column, block[k].color);
										}
										block.erase(block.begin() + k);
									}
								}
							}
						}
					}

					eraseBlockCount++;
				}
				if (eraseBlockCount == 9)
				{
					block.clear();
					GAME_MASTER->blockManager[0].gameResultState++;
					GAME_MASTER->blockManager[1].gameResultState++;
				}

			}
			else			//Win
			{
				for (size_t i = 0; i < block.size(); i++)
				{
					block[i].pos.y += 19;
					if (block[i].pos.y >= 1080 + 114)
					{
						block.erase(block.begin() + i);
					}
				}
			}
		}
		else if (sceneGame.isGameOver_two)
		{
			if (_playerNum == 0)	//Win
			{
				for (size_t i = 0; i < block.size(); i++)
				{
					block[i].pos.y += 19;
					if (block[i].pos.y >= 1080 + 114)
					{
						block.erase(block.begin() + i);
					}
				}
			}
			else			//Lose
			{
				if (++breakCount % 20 == 0)			// 1秒に一回上から壊す
				{
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (fixBlockData[i][j] == eraseBlockCount)
							{
								for (size_t k = 0; k < block.size(); k++)
								{
									if (block[k].row == j && block[k].column + 1 == i)
									{
										if (block[k].color != 7)
										{
											GAME_MASTER->effect[_playerNum].GenerateParticle(block[k].row, block[k].column, block[k].color);
										}
										block.erase(block.begin() + k);
									}
								}
							}
						}
					}

					eraseBlockCount++;
				}
				if (eraseBlockCount == 9)
				{
					block.clear();
					GAME_MASTER->blockManager[0].gameResultState++;
					GAME_MASTER->blockManager[1].gameResultState++;
				}

			}
		}
		break;
	case GAME_RESULT_MALTI::MULTI_DISPLAY_RESULT:
		if (sceneGame.isGameOver_one)
		{
			if (resultDisplayTimer <= 10)
			{
				resultDisplayAlpha += 0.1f;
			}
			if (resultDisplayAlpha == 0.1f)
			{
				pAudio->Play(SOUND->seHandle[SOUND->RESULT]);
			}

			resultDisplayPosY = easing::OutExp((float)resultDisplayTimer, 60.0f, 378.0f, 378.0f + 57.0f);
			++resultDisplayTimer;

			if (resultDisplayTimer >= 60)
			{
				gameResultState++;
				resultDisplayTimer = 0;
			}

		}
		else if (sceneGame.isGameOver_two)
		{
			if (resultDisplayTimer <= 10)
			{
				resultDisplayAlpha += 0.1f;
			}
			if (resultDisplayAlpha == 0.1f)
			{
				pAudio->Play(SOUND->seHandle[SOUND->RESULT]);
			}
			resultDisplayPosY = easing::OutExp((float)resultDisplayTimer, 60.0f, 387.0f, 387.0f + 57.0f);
			++resultDisplayTimer;

			if (resultDisplayTimer >= 60)
			{
				gameResultState++;
				resultDisplayTimer = 0;
			}
		}
		break;
	case GAME_RESULT_MALTI::MULTI_NEXT_BEHAVIOR:
		//TODO : 勝者に操作権を渡す
		if (pad[0].bLEFTt || pad[0].sX < 0)
		{
			if (++resultCount == 1)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				isResultSelectLeft = true;
			}
		}
		else
		{
			resultCount = 0;
		}

		if (pad[0].bRIGHTt || 0 < pad[0].sX)
		{
			if (++resultCount2 == 1)
			{
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
				isResultSelectLeft = false;
			}
		}
		else
		{
			resultCount2 = 0;
		}

		if (pad[0].bAt)
		{
			if (++resultCount3 == 1)
			{
				if (isResultSelectLeft)
				{
					//リトライ
					pAudio->Play(SOUND->seHandle[SOUND->OK]);
					sceneGame.goGameInit = true;
					sceneGame.goTitle = false;
					sceneGame.isStartInit = true;
					sceneGame.startTimer = 0;
				}
				else
				{
					//タイトル
					pAudio->Play(SOUND->seHandle[SOUND->OK]);
					sceneGame.goTitle = true;
					sceneGame.goGameInit = false;
					sceneGame.isStartInit = true;
					sceneGame.startTimer = 0;
				}
			}
		}
		else
		{
			resultCount3 = 0;
		}
		break;
	}
//		if (InputTrigger(XINPUT_A, _playerNum))
//		{
//			sceneGame.goTitle = true;
//			sceneGame.goGameInit = false;
//
//		}
//		if (InputTrigger(XINPUT_B, _playerNum))
//		{
//			sceneGame.goTitle = false;
//			sceneGame.goGameInit = true;
//		}

}

//-----------------------------------------------------------------------
//生成関数
//-----------------------------------------------------------------------
void BlockManager::GenerateBlock(int _row, int _column, int _color)
{
	Block pre;
	pre.color = _color;
	pre.row = _row;
	pre.column = _column;

	pre.Init();

	pre.pos = DirectX::XMFLOAT2(pre.row * 114.0f, pre.column * 114.0f);
	block.push_back(pre);
}

void BlockManager::SetStartBlock()
{
	for (int i = 6; i < 9; i++)
	{
		NextBlock _nb;
		int lastColor = -1;
		bool isLeft = false;
		for (int j = 0; j < ROW_MAX; j++)
		{
			NextBlock nextBlock[3];
			bool inData[3] = { false };
			for (size_t k = 0; k < block.size(); k++)
			{
				if (block[k].column == i - 1 && block[k].row == j - 1)	// left
				{
					nextBlock[0].color = block[k].color;
					nextBlock[0].par = MAX_PAR;
					inData[0] = true;
				}
				else if (block[k].column == i - 1 && block[k].row == 0)
				{
					nextBlock[0].color = -1;
					nextBlock[0].par = MAX_PAR;
					inData[0] = true;
					isLeft = true;
				}
				if (block[k].column == i - 1 && block[k].row == j)		// center
				{
					nextBlock[1].color = block[k].color;
					nextBlock[1].par = MIN_PAR;
					inData[1] = true;
					_nb = nextBlock[1];
				}
				if (block[k].column == i - 1 && block[k].row == j + 1)	// right
				{
					nextBlock[2].color = block[k].color;
					nextBlock[2].par = MAX_PAR;
					inData[2] = true;
				}
				else
				{
					for (int l = 0; l < colorMax; l++)
					{
						if (nextBlock[0].color != l && nextBlock[1].color != l)
						{
							nextBlock[2].color = l;
						}
					}
					for (int l = 0; l < colorMax; l++)
					{
						if (nextBlock[0].color != l && nextBlock[1].color != l && nextBlock[2].color != l)
						{
							nextBlock[1].color = l;
						}
					}
					nextBlock[0].par = END_MAX_PAR;
					nextBlock[1].par = END_MIN_PAR;
					nextBlock[2].par = END_MIN_PAR;
				}

				if (inData[0] == true && inData[1] == true && inData[2] == true)
				{
					break;
				}
			}

			if (isLeft)
			{
				for (int l = 0; l < colorMax; l++)
				{
					if (nextBlock[1].color != l && nextBlock[2].color != l)
					{
						nextBlock[0].color = l;
						break;
					}
				}
				for (int l = 0; l < colorMax; l++)
				{
					if (nextBlock[0].color != l && nextBlock[1].color != l && nextBlock[2].color != l)
					{
						nextBlock[1].color = l;
						break;
					}
				}
				nextBlock[0].par = END_MIN_PAR;
				nextBlock[1].par = END_MIN_PAR;
				nextBlock[2].par = END_MAX_PAR;
			}
			else
			{
				for (int l = 0; l < colorMax; l++)
				{
					if (nextBlock[0].color != l && nextBlock[1].color != l && nextBlock[2].color != l)
					{
						nextBlock[1].color = l;
					}
				}
			}

			// 乱数を発生させる
			int rnd_ = rand() % 100;
			int color_ = 0;
			int num = -1;
			if (rnd_ <= nextBlock[0].par)
			{
				color_ = nextBlock[0].color;
				num = 0;
			}
			else if (rnd_ <= nextBlock[0].par + nextBlock[1].par)
			{
				color_ = nextBlock[1].color;
				num = 1;
			}
			else if (rnd_ <= nextBlock[0].par + nextBlock[1].par + nextBlock[2].par)
			{
				color_ = nextBlock[2].color;
				num = 2;
			}
			while (lastColor == color_)
			{
				rnd_ = rand() % (100 - nextBlock[num].par);
				color_ = 0;
				int remainTwoColor[2] = { -1, -1 };
				int _num = 0;
				for (int i = 0; i < 3; i++)
				{
					if (i != num)
					{
						remainTwoColor[_num] = i;
						_num++;
					}
				}
				if (rnd_ <= nextBlock[remainTwoColor[0]].par)
				{
					color_ = nextBlock[remainTwoColor[0]].color;
}
				else if (rnd_ <= nextBlock[remainTwoColor[0]].par + nextBlock[remainTwoColor[1]].par)
				{
					color_ = nextBlock[remainTwoColor[1]].color;
				}
			}
			if (_nb.color == color_)
			{
				if (nextBlock[0].color == color_)
				{
					if (lastColor != -1)
					{
						if (nextBlock[1].color == lastColor)
						{
							color_ = nextBlock[2].color;
						}
						else if (nextBlock[2].color == lastColor)
						{
							color_ = nextBlock[1].color;
						}
						else
						{
							if (nextBlock[1].par < nextBlock[2].par)
							{
								color_ = nextBlock[2].color;
							}
							if (nextBlock[2].par < nextBlock[1].par)
							{
								color_ = nextBlock[1].color;
							}
						}
					}
					else
					{
						if (nextBlock[1].par < nextBlock[2].par)
						{
							color_ = nextBlock[2].color;
						}
						if (nextBlock[2].par < nextBlock[1].par)
						{
							color_ = nextBlock[1].color;
						}
					}
				}
				else if (nextBlock[1].color == color_)
				{
					if (lastColor != -1)
					{
						if (nextBlock[0].color == lastColor)
						{
							color_ = nextBlock[2].color;
						}
						else if (nextBlock[2].color == lastColor)
						{
							color_ = nextBlock[0].color;
						}
						else
						{
							if (nextBlock[0].par < nextBlock[2].par)
							{
								color_ = nextBlock[2].color;
							}
							if (nextBlock[2].par < nextBlock[0].par)
							{
								color_ = nextBlock[0].color;
							}
						}
					}
					else
					{
						if (nextBlock[0].par < nextBlock[2].par)
						{
							color_ = nextBlock[2].color;
						}
						if (nextBlock[2].par < nextBlock[0].par)
						{
							color_ = nextBlock[0].color;
						}
					}
				}
				else if (nextBlock[2].color == color_)
				{
					if (lastColor != -1)
					{
						if (nextBlock[0].color == lastColor)
						{
							color_ = nextBlock[1].color;
						}
						else if (nextBlock[1].color == lastColor)
						{
							color_ = nextBlock[0].color;
						}
						else
						{
							if (nextBlock[0].par < nextBlock[1].par)
							{
								color_ = nextBlock[1].color;
							}
							if (nextBlock[1].par < nextBlock[0].par)
							{
								color_ = nextBlock[0].color;
							}
						}
					}
					else
					{
						if (nextBlock[0].par < nextBlock[1].par)
						{
							color_ = nextBlock[1].color;
						}
						if (nextBlock[1].par < nextBlock[0].par)
						{
							color_ = nextBlock[0].color;
						}
					}
				}
				//_nb.color = color_;
			}
			lastColor = color_;
			GenerateBlock(j, i, color_);
		}
	}
}

void BlockManager::PushUpBoard(int _playerNum)
{
	bool chack[6];
	int cnt = 0;
	for (int i = 0; i < 6; i++)
	{
		chack[i] = false;
	}

	for (auto& b : block)
	{
		if (b.column == 9)
		{
			chack[cnt] = true;
			cnt++;
		}
		if (b.column == 0)
		{
			if (b.color == OBSTACLE && b.isFall)continue;
			if (!sceneGame.isTwoPlayer)
			{
				pAudio->Play(SOUND->seHandle[SOUND->GAME_END]);
				sceneGame.isGameOver_one = true;
			}
			else
			{
				pAudio->Play(SOUND->seHandle[SOUND->GAME_END]);
				if (_playerNum == 0)	sceneGame.isGameOver_one = true;
				else					sceneGame.isGameOver_two = true;
			}
		}
		b.column--;
		pushUp = true;
	}

//	assert(cnt == 6 && "cnt != 6");
}

void BlockManager::MoveBoard(int _playerNum)
{
	for (auto& b : block)
	{
		b.pos.y -= 114.0f / 6.0f;
	}
	pushUpCnt++;
	if (6 <= pushUpCnt)
	{
		GAME_MASTER->player[_playerNum].column--;
		GAME_MASTER->player[_playerNum].canMove = true;
		pushUpCnt = 0;
		pushUp = false;
		pushUpT = false;
		isPopRowLine = true;
		isLastChain = true;
		GAME_MASTER->ui[_playerNum].isTimerStop = false;
	}
}

void BlockManager::PopRowLine()
{
	//std::vector<Block> _block; // //TODO

	for (int i = 0; i < 6; i++)
	{
		desFunction._block[i] = desFunction.dummyBlock;
	}

	bool isPushBack[6] = { false };
	bool pushBack = false;

	for (int i = 0; i < ROW_MAX; i++)
	{
		pushBack = false;
		for (size_t j = 0; j < block.size(); j++)
		{
			if (block[j].row == i && block[j].column == 8)
			{
				//_block.push_back(block[j]);
				desFunction._block[i] = block[j];
				pushBack = true;
				break;
			}
		}
		if (!pushBack)
		{
			Block b;
			b.color = rand() % colorMax;
			b.column = 8;
			b.row = i;
			b.Init();
			//_block.push_back(b);
			desFunction._block[i] = b;
			isPushBack[i] = true;
		}
	}

	for (int i = 0; i < ROW_MAX; i++)
	{
		if (isPushBack[i])
		{
			if (0 < i && i < 5)
			{
				while (desFunction._block[i - 1].color == desFunction._block[i].color || desFunction._block[i + 1].color == desFunction._block[i].color)
				{
					desFunction._block[i].color = rand() % colorMax;
				}
			}
			else if (i == 0)
			{
				while (desFunction._block[i + 1].color == desFunction._block[i].color)
				{
					desFunction._block[i].color = rand() % colorMax;
				}
			}
			else if (i == 5)
			{
				while (desFunction._block[i - 1].color == desFunction._block[i].color)
				{
					desFunction._block[i].color = rand() % colorMax;
				}
			}
		}
	}

	int lastColor = -1;
	for (int i = 0; i < ROW_MAX; i++)
	{
		NextBlock nextBlock[3];
		int _color = -1;
		if (i == 0)
		{
			nextBlock[2].color = desFunction._block[i + 1].color;
			nextBlock[2].par = END_MAX_PAR;
			nextBlock[1].color = rand() % colorMax;
			while (nextBlock[1].color == desFunction._block[i].color || nextBlock[1].color == nextBlock[2].color)
			{
				nextBlock[1].color = rand() % colorMax;
			}
			nextBlock[1].par = END_MIN_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nextBlock[2].color && j != nextBlock[1].color && j != desFunction._block[i].color)
				{
					nextBlock[0].color = j;
				}
			}
			nextBlock[0].par = END_MIN_PAR;
		}
		else if (1 <= i && i <= 4)
		{
			nextBlock[0].color = desFunction._block[i - 1].color;
			nextBlock[0].par = MAX_PAR;
			nextBlock[2].color = desFunction._block[i + 1].color;
			nextBlock[2].par = MAX_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nextBlock[0].color && j != nextBlock[2].color && j != desFunction._block[i].color)
				{
					nextBlock[1].color = j;
				}
			}
			nextBlock[1].par = MIN_PAR;
		}
		else
		{
			nextBlock[0].color = desFunction._block[i - 1].color;
			nextBlock[0].par = END_MAX_PAR;
			nextBlock[1].color = rand() % colorMax;
			while (nextBlock[1].color == desFunction._block[i].color || nextBlock[1].color == nextBlock[0].color)
			{
				nextBlock[1].color = rand() % colorMax;
			}
			nextBlock[1].par = END_MIN_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nextBlock[0].color && j != nextBlock[1].color && j != desFunction._block[i].color)
				{
					nextBlock[2].color = j;
				}
			}
			nextBlock[2].par = END_MIN_PAR;
		}

		int rnd_ = (rand() % nextBlock[0].par) + (rand() % nextBlock[1].par) + (rand() % nextBlock[2].par);
		int num = -1;
		if (rnd_ <= nextBlock[0].par)
		{
			_color = nextBlock[0].color;
			num = 0;
		}
		else if (rnd_ <= nextBlock[0].par + nextBlock[1].par)
		{
			_color = nextBlock[1].color;
			num = 1;
		}
		else if (rnd_ <= nextBlock[0].par + nextBlock[1].par + nextBlock[2].par)
		{
			_color = nextBlock[2].color;
			num = 2;
		}

		while (lastColor != -1 && lastColor == _color)
		{
			int remainTwoColor[2] = { -1, -1 };
			int _num = 0;
			for (int j = 0; j < 3; j++)
			{
				if (j != num)
				{
					remainTwoColor[_num] = j;
					_num++;
				}
			}
			rnd_ = (rand() % nextBlock[remainTwoColor[0]].par) + (rand() % nextBlock[remainTwoColor[1]].par);
			num = -1;
			if (rnd_ <= nextBlock[remainTwoColor[0]].par)
			{
				_color = nextBlock[remainTwoColor[0]].color;
				num = remainTwoColor[0];
			}
			else if (rnd_ <= nextBlock[remainTwoColor[0]].par + nextBlock[remainTwoColor[1]].par)
			{
				_color = nextBlock[remainTwoColor[1]].color;
				num = remainTwoColor[1];
			}
		}

		lastColor = _color;
		GenerateBlock(i, 9, _color);
	}
}

void BlockManager::RagisterFallBlock(int _row, int _column, int _playerNum)
{
	int row_ = _row;
	int column_ = _column -1 ;
	int lastColumn_ = -1;
	for (int i = 0; i < COLUMN_MAX;i++) // 縦の想定されるMAX
	{
		for (size_t j = 0; j < GAME_MASTER->blockManager[_playerNum].block.size(); j++) // vectorの中身を見る
		{
			if (row_ == GAME_MASTER->blockManager[_playerNum].block[j].row && column_ == GAME_MASTER->blockManager[_playerNum].block[j].column) // 消えたブロックの上のブロックを見る
			{
				GAME_MASTER->blockManager[_playerNum].block[j].isFall = true;
				--column_;
				break;
			}
		}
		if (column_ < 0 || lastColumn_ == column_)
		{
			break;
		}

		lastColumn_ = column_;
	}
}

void BlockManager::RagisterChainBlock(int _playerNum)
{
	// 盤面を二次元配列に格納する
	//std::vector<std::vector<Block>> sortBlock; // //TODO
	//Block sortBlock[9][6];
	//
	//Block dummy;
	//dummy.row = -1;
	//dummy.column = -1;
	//dummy.color = -1;
	//dummy.elementNum = -1;

	//sortBlock[3][4] = dummy;

	desFunction.isData = false;

	desFunction._num = block.size();

	//std::vector<Block> rowBlock; // //TODO
	for (int i = 0; i < COLUMN_MAX; i++)
	{
		for (int j = 0; j < ROW_MAX; j++)
		{
			desFunction.sortBlock[i][j] = desFunction.dummyBlock;
			desFunction.isData = false;
			for (size_t k = 0; k < block.size(); k++)
			{
				if (block[k].row == j && block[k].column == i)
				{
					desFunction.isData = true;
					block[k].elementNum = k;
					//rowBlock.push_back(block[k]);
					desFunction.sortBlock[i][j] = block[k];
					break;
				}
			}
			if (!desFunction.isData)
			{
				//rowBlock.push_back(dummy);
				desFunction.sortBlock[i][j] = desFunction.dummyBlock;
			}
		}
		//sortBlock.push_back(rowBlock);
		//rowBlock.clear();
	}


	//int allElement[54] = { -1 };
	//std::vector<int> allElement; // TODO 
	for (int i = 0; i < 54; i++)
	{
		desFunction.allElement[i] = -1;
	}
	desFunction.allElementCnt = 0;

	// はんてい

	bool getChainStatus = false;
	DirectX::XMFLOAT2 passPos;
	for (int i = 0; i < COLUMN_MAX; i++)
	{
		for (int j = 0; j < ROW_MAX; j++)
		{
			if (desFunction._num != block.size())
			{
				desFunction._num = block.size();
			}
			if (desFunction.sortBlock[i][j].color == -1) continue;

			// ろーの判定
			if (j == 0)			// 左端
			{
				if (i == 0)
				{
					// 右
					if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, true, true, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, true, true, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else if (i == 8)
				{
					//上
					if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, false, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 右
					else if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, false, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else
				{
					// 右
					if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 上
					else if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, false);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
			}
			else if (j == 5)	// 右端
			{
				if (i == 0)
				{
					// 左
					if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, false, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, false, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else if (i == 8)
				{
					//上
					if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, false, false, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 左
					else if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, false, false, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else
				{
					// 左
					if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, false, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 上
					else if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, false, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, false, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
			}
			else				// その他
			{
				if (i == 0)
				{
					// 左
					if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 右
					else if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, false, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else if (i == 8)
				{
					// 左
					if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, false, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 上
					else if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, false, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 右
					else if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, false, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
				else
				{
					// 左
					if (desFunction.sortBlock[i][j - 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 右
					else if (desFunction.sortBlock[i][j + 1].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 上
					else if (desFunction.sortBlock[i - 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
					// 下
					else if (desFunction.sortBlock[i + 1][j].color == desFunction.sortBlock[i][j].color)
					{
						if (desFunction.sortBlock[i][j].color != 7)
						{
							desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[i][j].elementNum;
							desFunction.allElementCnt++;
							EraseObstacleBlock(j, i, _playerNum, true, true, true, true);
							if (!getChainStatus)
							{
								passPos = desFunction.sortBlock[i][j].pos;
								getChainStatus = true;
							}
							//allElement.push_back(desFunction.sortBlock[i][j].elementNum);
							//block.erase(block.begin() + sortBlock[i][j].elementNum);
							//BLOCK_MASTER->blockManager[0].RagisterFallBlock(sortBlock[i][j].row, sortBlock[i][j].column);
						}
					}
				}
			}
		}
	}
	if (getChainStatus)
	{
		GAME_MASTER->ui[_playerNum].SetChainDisplayInit(passPos);
	}

	isChainNow = false;

	if (0 < desFunction.allElementCnt)
	{
		std::sort(desFunction.allElement, desFunction.allElement + desFunction.allElementCnt);
		for (int i = 0; i < desFunction.allElementCnt; i++)
		{
			GAME_MASTER->blockManager[_playerNum].RagisterFallBlock(block[desFunction.allElement[i]].row, block[desFunction.allElement[i]].column, _playerNum);
			auto it = block.data() + +desFunction.allElement[i];
			if (it->color != 7)
			{
				GAME_MASTER->effect[_playerNum].GenerateParticle(it->row, it->column, it->color);
			}
			block.erase(block.begin() + desFunction.allElement[i]);
		//	SetXInputVibration(45000, 45000, 10, _playerNum);
			
			eraseCount++;
			isChainNow = true;

			if (i < desFunction.allElementCnt - 1)
			{
				for (int j = i+1; j < desFunction.allElementCnt; j++)
				{
					desFunction.allElement[j]--;
				}
			}
		}
	}

	if (isChainNow)
	{
		pAudio->Play(SOUND->seHandle[SOUND->DESTRUCTION1]);
	}
}

void BlockManager::JudgeUpToOnBlock(int _playerNum)
{
	int count = 0;
	GAME_MASTER->effect[_playerNum].isLastAleat = GAME_MASTER->effect[_playerNum].isAleat;
	for (size_t i = 0; i < block.size(); i++)
	{
		if (block[i].column == 0)
		{
			count++;
		}
	}
	if (count == 0)
	{
		GAME_MASTER->effect[_playerNum].isAleat = false;
	}
	else
	{
		GAME_MASTER->effect[_playerNum].isAleat = true;
	}
}

bool BlockManager::JudgeOverTheCeiling()
{
	if (nowObstacleFall) return false;

	int count = 0;
	for (size_t i = 0; i < block.size(); i++)
	{
		if (block[i].column == -1)
		{
			count++;
		}
	}

	if (count == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void BlockManager::TwoPlayDraw(int _playerNum)
{
	switch (_playerNum)
	{
	case 0:
		for (auto &it : block)
		{
			if (9 <= it.column)
			{
				continue;
			}
			sprBlock->Draw(it.pos.x + ADJUST + TWO_PLAY_ONE_ORIJIN_X, it.pos.y + ADJUST + ORIJIN_Y + shakePos.y, static_cast<float>(it.SIZE_X), static_cast<float>(it.SIZE_Y), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.color);
		}
		break;
	case 1:
		for (auto &it : block)
		{
			if (9 <= it.column)
			{
				continue;
			}
			sprBlock->Draw(it.pos.x + ADJUST + TWO_PLAY_TWO_ORIJIN_X, it.pos.y + ADJUST + ORIJIN_Y + shakePos2.y, static_cast<float>(it.SIZE_X), static_cast<float>(it.SIZE_Y), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.color);
		}
		break;
	default:
		break;
	}
}

/***********************************************************************
お邪魔ブロック関係の関数
***********************************************************************/
void BlockManager::AttackOfObstacleRelation(int _playerNum)
{
	// おじゃまを与える方
	switch (_playerNum)
	{
	case 0:
		if (chainCount >= 2)
		{
			GAME_MASTER->blockManager[1].obstacleCount++;				// おじゃまカウント＋＋
			GAME_MASTER->blockManager[1].obstacleFallWaitTime = 0;		// 落下するまでの猶予時間
			GAME_MASTER->blockManager[1].obstacleFallWaiting = true;	// 落下待機フラグ
		}
		break;
	case 1:
		if (chainCount >= 2)
		{
			GAME_MASTER->blockManager[0].obstacleCount++;				// おじゃまカウント＋＋
			GAME_MASTER->blockManager[0].obstacleFallWaitTime = 0;		// 落下するまでの猶予時間
			GAME_MASTER->blockManager[0].obstacleFallWaiting = true;	// 落下待機フラグ
		}
		break;
	}
}

void BlockManager::ReceiveOfObstacleRelation(int _playerNum)
{
	switch (_playerNum)
	{
	case 0:
		if (/*GAME_MASTER->blockManager[_playerNum].*/nowObstacleFall)
		{
			if (++fallObstacleCount >= 60)
			{
				fallObstacleCount = 0;
				nowObstacleFall = false;
			}
		}

		break;
	case 1:
		if (/*GAME_MASTER->blockManager[_playerNum].*/nowObstacleFall)
		{
			if (++fallObstacleCount >= 60)
			{
				fallObstacleCount = 0;
				nowObstacleFall = false;
			}
		}
		break;
	}

	int num = 0;
	switch (_playerNum)
	{
	case 0:
		if (!/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaiting) return;

		if (++/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaitTime >= 180)
		{
			// お邪魔ブロックを降らせる関数を呼び出す
			if (pushUp)return;
			num = SetNumObstacle(obstacleCount);
			FallObstacle(_playerNum, /*GAME_MASTER->blockManager[_playerNum].*/num);
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaitTime = 0;
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleCount = 0;
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaiting = false;
			/*GAME_MASTER->blockManager[_playerNum].*/nowObstacleFall = true;
			//fallObstacleCount = 0;
		}
		break;
	case 1:
		if (!/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaiting) return;

		if (++/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaitTime >= 180)
		{
			// お邪魔ブロックを降らせる関数を呼び出す
			if (pushUp)return;
			num = SetNumObstacle(obstacleCount);
			FallObstacle(_playerNum, /*GAME_MASTER->blockManager[_playerNum].*/num);
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaitTime = 0;
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleCount = 0;
			/*GAME_MASTER->blockManager[_playerNum].*/obstacleFallWaiting = false;
			/*GAME_MASTER->blockManager[_playerNum].*/nowObstacleFall = true;
			//fallObstacleCount = 0;
		}
		break;
	}

}

int	BlockManager::SetNumObstacle(int _count)
{
#if 0
	switch (_count)
	{
	case 1:
		return 0;
		break;
	case 2:
		return 1;
		break;
	case 3:
		return 2;
		break;
	case 4:
		return 4;
		break;
	case 5:
		return 6;
		break;
	case 6:
		return 9;
		break;
	case 7:
		return 12;
		break;
	case 8:
		return 15;
		break;
	default:
		return 15;
		break;
	}
#else
	switch (_count)
	{
	case 1:
		return 0;
		break;
	case 2:
		return 1;
		break;
	case 3:
		return 2;
		break;
	case 4:
		return 4;
		break;
	case 5:
		return 6;
		break;
	case 6:
		return 8;
		break;
	case 7:
		return 11;
		break;
	case 8:
		return 14;
		break;
	case 9:
		return 18;
	default:
		return 20;
		break;
	}
#endif
}

bool BlockManager::JudgeIsFallObstacle()
{
	int count = 0;
	for (size_t i = 0; i < block.size(); i++)
	{
		if (block[i].color == 7 && block[i].isFall)
		{
			count++;
		}
	}
	if (count == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void BlockManager::FallObstacle(int _playerNum, int _fallObstacleNum)
{
	int checkBoard[10][6];
	int fallObstacleBlockRow[20];
	int enable = 0;
	int obstacle = 1;
	int dummy = -1;
	int count = 0;

	// checkBoardを初期化
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			checkBoard[i][j] = dummy;
		}
	}
	// 落とす予定地を初期化しておく
	for (auto& it : fallObstacleBlockRow)
	{
		it = dummy;
	}


	for (int k = 0; k < _fallObstacleNum; k++)
	{
		// vectorの中身を見る
		for (size_t i = 0; i < block.size(); i++)
		{
			checkBoard[block[i].column][block[i].row] = enable;	// 要素があるところにはenableを入れる
		}

		// 左下から見て、dummyがあるところの上にお邪魔を生成する
		bool getArea = false;
		for (int i = 8; i > -1; i--)
		{
			for (int j = 5; j > -1; j--)
			{
				if (checkBoard[i][j] == dummy)
				{
					fallObstacleBlockRow[count++] = j;
					checkBoard[i][j] = enable;
					getArea = true;
				}
				if (getArea)break;
			}
			if (getArea)break;
		}
	}

	int overlapCount[6] = { 0 };// 被った回数
	int generateRowSave[20];	// 生成したrowを保持する
	for (auto& it : generateRowSave)
	{
		it = -1;
	}

	// お邪魔ブロックを生成する
	int generateCount = 0;
	for (int i = 0; i < 20; i++)
	{
		if (fallObstacleBlockRow[i] == dummy)break;

		for (int j = 0; j < 20; j++)
		{
			if (fallObstacleBlockRow[i] == generateRowSave[j])
			{
				overlapCount[fallObstacleBlockRow[i]]++;
			}
		}
		GenerateBlock(fallObstacleBlockRow[i], -2 - overlapCount[fallObstacleBlockRow[i]], COLOR::OBSTACLE);
		block[block.size() - 1].isFall = true;
		generateRowSave[generateCount++] = fallObstacleBlockRow[i];
	}
}

void BlockManager::EraseObstacleBlock(int _row, int _column, int _playerNum, bool _isSearchUp, bool _isSearchRight, bool _isSearchDown, bool _isSearchLeft)
{
	// 隣り合うカラーがObstacle(お邪魔)なら消す
	if (_isSearchUp)
	{
		if (desFunction.sortBlock[_column - 1][_row].color == COLOR::OBSTACLE)
		{
			desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[_column - 1][_row].elementNum;
			desFunction.allElementCnt++;
		}
	}

	if (_isSearchRight)
	{
		if (desFunction.sortBlock[_column][_row + 1].color == COLOR::OBSTACLE)
		{
			desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[_column][_row + 1].elementNum;
			desFunction.allElementCnt++;
		}
	}

	if (_isSearchDown)
	{
		if (desFunction.sortBlock[_column + 1][_row].color == COLOR::OBSTACLE)
		{
			desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[_column + 1][_row].elementNum;
			desFunction.allElementCnt++;
		}
	}

	if (_isSearchLeft)
	{
		if (desFunction.sortBlock[_column][_row - 1].color == COLOR::OBSTACLE)
		{
			desFunction.allElement[desFunction.allElementCnt] = desFunction.sortBlock[_column][_row - 1].elementNum;
			desFunction.allElementCnt++;
		}
	}
}