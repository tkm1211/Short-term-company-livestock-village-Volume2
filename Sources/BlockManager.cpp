#include "BlockManager.h"
#include "Resource.h"

#include <input_device.h> //TODO:後で消す
#include <array>
#include "SceneManager.h"
#include "SceneSelect.h"
#include "Production.h"
#include "UI.h"
#include "Effect.h"
#include "Sound.h"
#include <imgui.h>
#include "Player.h"

//#define DEBUG_BLOCKS

class Player;
/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

std::array<BlockManager, 2> regularBlockManager;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void BlockManager::Init(int _pn)
{
	InitializeVariables(_pn);

#ifdef DEBUG_BLOCKS
	GenerateBlock(0, 5, Color::Green, false);
	GenerateBlock(1, 5, Color::Yellow, false);
	GenerateBlock(2, 5, Color::LightBlue, false);
	GenerateBlock(3, 5, Color::Red, false);
	GenerateBlock(4, 5, Color::Green, false);
	GenerateBlock(5, 5, Color::Yellow, false);

	GenerateBlock(0, 6, Color::Yellow, false);
	GenerateBlock(1, 6, Color::LightBlue, false);
	GenerateBlock(2, 6, Color::Red, false);
	GenerateBlock(3, 6, Color::Green, false);
	GenerateBlock(4, 6, Color::Yellow, false);
	GenerateBlock(5, 6, Color::LightBlue, false);

	GenerateBlock(0, 7, Color::LightBlue, false);
	GenerateBlock(1, 7, Color::Red, false);
	GenerateBlock(2, 7, Color::Green, false);
	GenerateBlock(3, 7, Color::Red, false);
	GenerateBlock(4, 7, Color::LightBlue, false);
	GenerateBlock(5, 7, Color::Yellow, false);

	GenerateBlock(0, 8, Color::Red, false);
	GenerateBlock(1, 8, Color::Green, false);
	GenerateBlock(2, 8, Color::Yellow, false);
	GenerateBlock(3, 8, Color::LightBlue, false);
	GenerateBlock(4, 8, Color::Green, false);
	GenerateBlock(5, 8, Color::Red, false);


#else
	const int difficultyColorNum[7] = { 4, 5, 5, 6, 6, 7, 7 };
	colorMax = difficultyColorNum[sceneSelect.GetLevel(_pn) - 1];

	int lastRnd = rand() % colorMax;
	GenerateBlock(0, 5, lastRnd, false);

	for (int i = 1; i < 6; i++)
	{
		int rnd = rand() % colorMax;
		while (rnd == lastRnd)
		{
			rnd = rand() % colorMax;
		}
		lastRnd = rnd;
		GenerateBlock(i, 5, rnd, false);
	}

	SetStartBlocks();
#endif

	PopRowLineProcess();


}

void BlockManager::InitializeVariables(int _pn)
{
	// Member variable initialize.
	sprBlock = RESOURCE->GetSpriteData(Resource::Texture::Block);

	const int difficultyColorNum[7] = { 4, 5, 5, 6, 6, 7, 7 };
	colorMax = difficultyColorNum[sceneSelect.GetLevel(_pn) - 1];

	for (auto& it : blocks)
	{
		it.Init();
	}

	status = State::Wait;

	for (auto& it : waitTime) it = 0;

	chainCount = 0;
	pushingCount = 0;
	eraseBlockCount = 0;
	obstacleNum = 0;
	obstacleKeepTime = 0;
	fallObstacleNum = 0;

	isChainContinued = false;
	isPushing = false;
	isPushUpByGauge = false;
	isChainAfterPushUp = false;
	isObstacleKeeping = false;
	fallObstacleNow = false;


	// GameResult Variables
	gameResultState = GameResultSingle::Start;
	shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
	shakePos2 = DirectX::XMFLOAT2(0.0f, 0.0f);
	shakeTimer = 0;
	eraseBlockCount = 0;
	isInitBreak = false;
	resultDisplayAlpha = 0.0f;
	resultDisplayTimer = 0;
	resultDisplayPos.x = 615;
	resultDisplayPos.y = 288;
	addResultPosY = 471;
	resultDisplayPosFirst.x = 615;
	resultDisplayPosFirst.y = 378;
	scoreDisplayAlpha = 0.0f;
	scoreDisplayPosY = 546.0f;
	resultDisplayMultiAlpha = 0.0f;
	resultCnt[0] = 0;
	resultCnt[1] = 0;
	resultCnt[2] = 0;
	isResultSelectLeft = true;

	rankingLogoDisplayAlpha = 0.0f;
	rankingLogoDisplayPosY = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		rankingScoreDisplayAlpha[i] = 0.0f;
		rankingScoreDisplayPosY[i] = 0.0f;
	}

}


void BlockManager::Uninit()
{
	for (auto& it : blocks)
	{
		it.Uninit();
	}

}

void BlockManager::Update(int _pn)
{

	// 一人用の更新関数
	if (IF_SINGLE_NOW)
	{
		if (sceneSingleGame.isGameover)
		{
			ProcessOfGameResultOnePlayer();
		}
		else
		{
			ProcessOfSingleGame();
		}
	}
	else if (IF_MULTI_NOW)
	{
		if (sceneMultiGame.isGameover[0] || sceneMultiGame.isGameover[1])
		{
			ProcessOfGameResultTwoPlayer(_pn);
		}
		else
		{
			ProcessOfMultiGame(_pn);
		}
	}

}

void BlockManager::Draw()
{
	sprBlock->Begin();
	for (auto& it : blocks)
	{
		if (9 <= it.GetColumn() || !it.GetIsExist())
		{
			continue;
		}

		sprBlock->Draw(it.GetPos().x + ADJUST + SINGLE_CORRECTION_X, it.GetPos().y + ADJUST + SINGLE_CORRECTION_Y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
	}
	sprBlock->End();
}

void BlockManager::DrawOfSingle()
{
	sprBlock->Begin();
	for (auto& it : blocks)
	{
		if (9 <= it.GetColumn() || !it.GetIsExist())
		{
			continue;
		}

		sprBlock->Draw(it.GetPos().x + ADJUST + SINGLE_CORRECTION_X, it.GetPos().y + ADJUST + SINGLE_CORRECTION_Y + shakePos.y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
	}
	sprBlock->End();
}

void BlockManager::DrawOfMulti(int _pn)
{
	sprBlock->Begin();
	switch (_pn)
	{
	case 0:
		for (auto& it : blocks)
		{
			if (9 <= it.GetColumn() || !it.GetIsExist())
			{
				continue;
			}

			sprBlock->Draw(it.GetPos().x + ADJUST + GameUI::MULTIPLAY_ONE_ORIJIN_X, it.GetPos().y + ADJUST + GameUI::MULTI_CORRECTION_Y + shakePos.y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
		}
		break;

	case 1:
		for (auto& it : blocks)
		{
			if (9 <= it.GetColumn() || !it.GetIsExist())
			{
				continue;
			}

			sprBlock->Draw(it.GetPos().x + ADJUST + GameUI::MULTIPLAY_TWO_ORIJIN_X, it.GetPos().y + ADJUST + GameUI::MULTI_CORRECTION_Y + shakePos2.y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
		}
		break;
	}
	sprBlock->End();
}





//-----------------------------------------------------------------------

//	各種関数

//-----------------------------------------------------------------------
/*-------------------------------------------*/
// シングルゲームの更新関数
/*-------------------------------------------*/
void BlockManager::ProcessOfSingleGame()
{
	// ブロックを二次元配列へソートする
	SetSortBlocks();


	switch (status)
	{
	case BlockManager::Wait:
		WaitProcess();
		break;
	case BlockManager::Break:
		BreakProcess();
		break;
	case BlockManager::Chain:
		ChainProcess(0);
		break;
	case BlockManager::PushUp:
		PushUpProcess(0);
		break;
	case BlockManager::PopRowLine:
		PopRowLineProcess();
		break;
	case BlockManager::CheckUpCombo:
		CheckUpComboProcess(0);
		break;
	default:
		break;
	}

	if (isPushUpByGauge && !isPushing)
	{
		SetStatus(State::PushUp);
		isPushUpByGauge = false;
	}

	for (auto& it : blocks)
	{
		it.Update(0);
	}
}

/*-------------------------------------------*/
// マルチゲームの更新関数
/*-------------------------------------------*/
void BlockManager::ProcessOfMultiGame(int _pn)
{
	// ブロックを二次元配列へソートする
	SetSortBlocks();


	switch (status)
	{
	case BlockManager::Wait:
		WaitProcess();
		break;
	case BlockManager::Break:
		BreakProcess();
		break;
	case BlockManager::Chain:
		ChainProcess(_pn);
		break;
	case BlockManager::PushUp:
		PushUpProcess(_pn);
		break;
	case BlockManager::PopRowLine:
		PopRowLineProcess();
		break;
	case BlockManager::CheckUpCombo:
		CheckUpComboProcess(_pn);
		break;
	case BlockManager::FallObstacle:
		FallObstacleProcess(_pn);
		break;
	default:
		break;
	}

	if (isPushUpByGauge && !isPushing)
	{
		SetStatus(State::PushUp);
		isPushUpByGauge = false;
	}

	SetObstacleWhenCountReaches(_pn);
	fallObstacleNum = SetNumOfObstacle(obstacleNum);

	for (auto& it : blocks)
	{
		it.Update(_pn);
	}

}

/*-------------------------------------------*/
// シングルゲームのリザルト関数
/*-------------------------------------------*/

void BlockManager::ProcessOfGameResultOnePlayer()
{
	switch (gameResultState)
	{
	case GameResultSingle::Start:
		// ランキングを更新する
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::GAME_END].get());
		regularGameUI[0].FileRead();
		regularGameUI[0].RankingSort();
		regularGameUI[0].FileWrite();
		gameResultState++;
		//break;
	case GameResultSingle::Shake:
		if (shakeTimer % 4 >= 2) {
			shakePos.y = -5.0f;
		}
		else {
			shakePos.y = 5.0f;
		}
		if (++shakeTimer >= 20)
		{
			gameResultState++;
			shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
		}
		break;
	case GameResultSingle::BreakBlocks:
		if (!isInitBreak)
		{
			breakCnt = 0;
			for (int i = 0; i < 10; i++) // column + 上にいった一つ
			{
				for (int j = 0; j < 6; j++)
				{
					fixBlockData[i][j] = -1;
				}
			}
			for (size_t i = 0; i < blocks.size(); i++)
			{
				fixBlockData[blocks[i].GetColumn() + 1][blocks[i].GetRow()] = blocks[i].GetColumn() + 1;
			}
			isInitBreak = true;
		}
		else
		{
			if (++breakCnt % 20 == 0)			// 1秒に一回上から壊す
			{
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 6; j++)
					{
						if (fixBlockData[i][j] == eraseBlockCount)
						{
							for (size_t k = 0; k < blocks.size(); k++)
							{
								if (blocks[k].GetRow() == j && blocks[k].GetColumn() + 1 == i)
								{
									if (blocks[k].GetColor() != 7)
									{
										regularEffects[0].GenerateParticle(blocks[k].GetRow(), blocks[k].GetColumn(), blocks[k].GetColor());
									}
									blocks[k].SetIsExist(false);
								}
							}
						}
					}
				}

				eraseBlockCount++;
			}
			if (eraseBlockCount == 9)
			{
				Block dummy;
				dummy.SetPosition(DirectX::XMFLOAT2(-Block::SIZE_X, -Block::SIZE_Y));
				dummy.SetIsExist(false);
				blocks.fill(dummy);
				gameResultState++;
			}
		}
		break;
	case GameResultSingle::DisplayResult:
		if (resultDisplayTimer <= 10) {
			resultDisplayAlpha += 0.1f;
		}
		if (resultDisplayAlpha == 0.1f){
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		resultDisplayPos.y = easing::OutExp((float)resultDisplayTimer, 60, 250 - 57, 250);
		addResultPosY = easing::OutExp((float)resultDisplayTimer, 60, 350, 350 + 57);
		++resultDisplayTimer;

		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
		}
		break;
	case GameResultSingle::DisplayScore:
		if (resultDisplayTimer <= 10){
			scoreDisplayAlpha += 0.1f;
		}
		if (scoreDisplayAlpha == 0.1f)
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}

		scoreDisplayPosY = easing::OutExp((float)resultDisplayTimer, 60, 400, 400 + 57);

		++resultDisplayTimer;
		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		break;
	case GameResultSingle::DisplayRanking:
		if (resultDisplayTimer <= 10){
			rankingLogoDisplayAlpha += 0.1f;
		}
		if (rankingLogoDisplayAlpha == 0.1f){
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		rankingLogoDisplayPosY = easing::OutExp((float)resultDisplayTimer, 60, 475 - 57 + 100 * 1, 475 + 100 * 1);
		++resultDisplayTimer;

		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
		}
		break;
	case GameResultSingle::DisplayFirst:
		if (resultDisplayTimer <= 10){
			rankingScoreDisplayAlpha[0] += 0.1f;
		}
		if (rankingScoreDisplayAlpha[0] == 0.1f){
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}

		rankingScoreDisplayPosY[0] = easing::OutExp((float)resultDisplayTimer, 60, 460 + 110 * 2, 460 + 57 + 110 * 2);

		++resultDisplayTimer;
		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		break;
	case GameResultSingle::DisplaySecond:
		if (resultDisplayTimer <= 10)
		{
			rankingScoreDisplayAlpha[1] += 0.1f;
		}
		if (rankingScoreDisplayAlpha[1] == 0.1f)
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}

		rankingScoreDisplayPosY[1] = easing::OutExp((float)resultDisplayTimer, 60, 460 + 110 * 3, 460 + 57 + 110 * 3);

		++resultDisplayTimer;
		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		break;
	case GameResultSingle::DisplayThird:
		if (resultDisplayTimer <= 10)
		{
			rankingScoreDisplayAlpha[2] += 0.1f;
		}
		if (rankingScoreDisplayAlpha[2] == 0.1f)
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}

		rankingScoreDisplayPosY[2] = easing::OutExp((float)resultDisplayTimer, 60, 460 + 110 * 4, 460 + 57 + 110 * 4);

		++resultDisplayTimer;
		if (resultDisplayTimer >= 60)
		{
			gameResultState++;
			resultDisplayTimer = 0;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
		}
		break;
	case GameResultSingle::ChoiceBehavior:
		static int count;
		static int count2;
		static int count3;
		if (pad[0].bLEFTt)
		{
			if (++count == 1)
			{
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				isResultSelectLeft = true;
			}
		}
		else{
			count = 0;
		}

		if (pad[0].bRIGHTt)
		{
			if (++count2 == 1)
			{
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				isResultSelectLeft = false;
			}
		}
		else{
			count2 = 0;
		}

		if (pad[0].bAt)
		{
			if (++count3 == 1)
			{
				if (isResultSelectLeft)
				{
					//リトライ
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					//sceneGame.goGameInit = true;
					//sceneGame.goTitle = false;
					//sceneGame.isStartInit = true;
					//sceneGame.startTimer = 0;
					if (!PRODUCTION->CheckFlag(GO_SINGLEGAME)) {
						PRODUCTION->SetOn(GO_SINGLEGAME);
						PRODUCTION->Start();
					}

				}
				else
				{
					//タイトル
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					//sceneGame.goTitle = true;
					//sceneGame.goGameInit = false;
					//sceneGame.isStartInit = true;
					//sceneGame.startTimer = 0;
					if (!PRODUCTION->CheckFlag(GO_TITLE)) {
						PRODUCTION->SetOn(GO_TITLE);
						PRODUCTION->Start();
					}

				}
			}
		}
		else
		{
			count3 = 0;
		}
		break;
	}
}

/*-------------------------------------------*/
// マルチゲームのリザルト関数
/*-------------------------------------------*/
void BlockManager::ProcessOfGameResultTwoPlayer(int _pn)
{
	switch (gameResultState)
	{
	case GameResultMulti::m_Start:
		regularGameUI[0].isTimerStop = true;
		regularGameUI[1].isTimerStop = true;
		gameResultState++;
		//break;
	case GameResultMulti::m_Shake:
		if (sceneMultiGame.isGameover[0])
		{
			if (shakeTimer % 4 >= 2){
				shakePos.y = -5.0f;
			}
			else{
				shakePos.y = 5.0f;
			}
			if (++shakeTimer >= 20) 
			{
				gameResultState++;
				shakePos = DirectX::XMFLOAT2(0.0f, 0.0f);
			}
		}
		else if (sceneMultiGame.isGameover[1])
		{
			if (shakeTimer % 4 >= 2) {
				shakePos2.y = -5.0f;
			}
			else {
				shakePos2.y = 5.0f;
			}
			if (++shakeTimer >= 20)
			{
				gameResultState++;
				shakePos2 = DirectX::XMFLOAT2(0.0f, 0.0f);
			}
		}
		break;
	case GameResultMulti::m_RagisterBreakBlock:
		if (sceneMultiGame.isGameover[0])
		{
			if (_pn == 0)
			{
				// Loser
				if (!isInitBreak)
				{
					breakCnt = 0;
					for (int i = 0; i < 10; i++) //column + 上にいった一つ
					{
						for (int j = 0; j < 6; j++)
						{
							fixBlockData[i][j] = -1;
						}
					}
					for (size_t i = 0; i < blocks.size(); i++)
					{
						fixBlockData[blocks[i].GetColumn() + 1][blocks[i].GetRow()] = blocks[i].GetColumn() + 1;
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
		else if (sceneMultiGame.isGameover[1])
		{
			if (_pn == 0)
			{
				gameResultState++;
				break;	//WIN側は何もしない
			}
			else
			{
				// Loser
				if (!isInitBreak)
				{
					breakCnt = 0;
					for (int i = 0; i < 10; i++) //column + 上にいった一つ
					{
						for (int j = 0; j < 6; j++)
						{
							fixBlockData[i][j] = -1;
						}
					}
					for (size_t i = 0; i < blocks.size(); i++)
					{
						fixBlockData[blocks[i].GetColumn() + 1][blocks[i].GetRow()] = blocks[i].GetColumn() + 1;
						assert(gameResultState == 2 && "gameResultState error");
					}
					isInitBreak = true;
					gameResultState++;
				}
			}
		}

		break;
	case GameResultMulti::m_BreakBlock:
		if (sceneMultiGame.isGameover[0])
		{
			if (_pn == 0)	//Lose
			{
				if (++breakCnt % 20 == 0)			// 1秒に一回上から壊す
				{
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (fixBlockData[i][j] == eraseBlockCount)
							{
								for (size_t k = 0; k < blocks.size(); k++)
								{
									if (blocks[k].GetRow() == j && blocks[k].GetColumn() + 1 == i)
									{
										if (blocks[k].GetColor() != 7)
										{
											regularEffects[_pn].GenerateParticle(blocks[k].GetRow(), blocks[k].GetColumn(), blocks[k].GetColor());
										}
										blocks[k].SetIsExist(false);
									}
								}
							}
						}
					}

					eraseBlockCount++;
				}
				if (eraseBlockCount == 9)
				{
					Block dummy;
					dummy.SetIsExist(false);
					blocks.fill(dummy);
					regularBlockManager[0].gameResultState++;
					regularBlockManager[1].gameResultState++;
				}

			}
			else			//Win
			{
				// なにもしない
			}
		}
		else if (sceneMultiGame.isGameover[1])
		{
			if (_pn == 0)	//Win
			{
				// なにもしない
			}
			else			//Lose
			{
				if (++breakCnt % 20 == 0)			// 1秒に一回上から壊す
				{
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (fixBlockData[i][j] == eraseBlockCount)
							{
								for (size_t k = 0; k < blocks.size(); k++)
								{
									if (blocks[k].GetRow() == j && blocks[k].GetColumn() + 1 == i)
									{
										if (blocks[k].GetColor() != 7)
										{
											regularEffects[_pn].GenerateParticle(blocks[k].GetRow(), blocks[k].GetColumn(), blocks[k].GetColor());
										}
										blocks[k].SetIsExist(false);
									}
								}
							}
						}
					}

					eraseBlockCount++;
				}
				if (eraseBlockCount == 9)
				{
					Block dummy;
					dummy.SetIsExist(false);
					blocks.fill(dummy);
					regularBlockManager[0].gameResultState++;
					regularBlockManager[1].gameResultState++;
				}
			}
		}

		break;
	case GameResultMulti::m_DisplayResult:
		if (sceneMultiGame.isGameover[0])
		{
			if (resultDisplayTimer <= 10)
			{
				resultDisplayAlpha += 0.1f;
			}
			if (resultDisplayAlpha == 0.1f)
			{
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
			}

			resultDisplayPos.y = easing::OutExp((float)resultDisplayTimer, 60.0f, 378.0f, 378.0f + 57.0f);
			++resultDisplayTimer;

			if (resultDisplayTimer >= 60)
			{
				gameResultState++;
				resultDisplayTimer = 0;
			}
		}
		else if (sceneMultiGame.isGameover[1])
		{
			if (resultDisplayTimer <= 10) {
				resultDisplayAlpha += 0.1f;
			}
			if (resultDisplayAlpha == 0.1f) {
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::RESULT].get());
			}
			resultDisplayPos.y = easing::OutExp((float)resultDisplayTimer, 60.0f, 387.0f, 387.0f + 57.0f);
			++resultDisplayTimer;

			if (resultDisplayTimer >= 60)
			{
				gameResultState++;
				resultDisplayTimer = 0;
			}
		}
		break;
	case GameResultMulti::m_ChoiceBehavior:
		if (pad[0].bLEFTt)
		{
			if (++resultCnt[0] == 1)
			{
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				isResultSelectLeft = true;
			}
		}
		else
		{
			resultCnt[0] = 0;
		}

		if (pad[0].bRIGHTt)
		{
			if (++resultCnt[1] == 1)
			{
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				isResultSelectLeft = false;
			}
		}
		else
		{
			resultCnt[1] = 0;
		}

		if (pad[0].bAt)
		{
			if (++resultCnt[2] == 1)
			{
				if (isResultSelectLeft)
				{
					//リトライ
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
					if (!PRODUCTION->CheckFlag(GO_MULTIGAME)) {
						PRODUCTION->SetOn(GO_MULTIGAME);
						PRODUCTION->Start();
					}
				}
				else
				{
					//タイトル
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
					if (!PRODUCTION->CheckFlag(GO_TITLE)) {
						PRODUCTION->SetOn(GO_TITLE);
						PRODUCTION->Start();
					}
				}
			}
		}
		else
		{
			resultCnt[2] = 0;
		}
		break;
	}
}

/*-------------------------------------------*/
// ブロックの生成関数
/*-------------------------------------------*/
void BlockManager::GenerateBlock(int _row, int _column, int _color, bool _isFall)
{
	for (auto& it : blocks)
	{
		if (it.GetIsExist()) continue;

		// If, GetIsExist is False.
		it.GenerateMe(_row, _column, _color, _isFall);
		break;
	}
}

/*-------------------------------------------*/
// スタート時のブロック設定
/*-------------------------------------------*/
void BlockManager::SetStartBlocks()
{
	struct NextBlock
	{
		int color;
		int par;
		NextBlock() :color(-1), par(-1) {}
	};
	for (int i = 6; i < BOARD_COLUMN_MAX; i++)
	{
		NextBlock _nb;

		int lastColor = -1;
		bool isLeft = false;
		for (int j = 0; j < ROW_MAX; j++)
		{
			NextBlock nextBlock[3]{};
			bool inData[3] = { false };
			for (size_t k = 0; k < blocks.size(); k++)
			{
				if (blocks[k].GetColumn() == i - 1 && blocks[k].GetRow() == j - 1)	// left
				{
					nextBlock[0].color = blocks[k].GetColor();
					nextBlock[0].par = MAX_PAR;
					inData[0] = true;
				}
				else if (blocks[k].GetColumn() == i - 1 && blocks[k].GetRow() == 0)
				{
					nextBlock[0].color = -1;
					nextBlock[0].par = MAX_PAR;
					inData[0] = true;
					isLeft = true;
				}
				if (blocks[k].GetColumn() == i - 1 && blocks[k].GetRow() == j)		// center
				{
					nextBlock[1].color = blocks[k].GetColor();
					nextBlock[1].par = MIN_PAR;
					inData[1] = true;
					_nb = nextBlock[1];
				}
				if (blocks[k].GetColumn() == i - 1 && blocks[k].GetRow() == j + 1)	// right
				{
					nextBlock[2].color = blocks[k].GetColor();
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
			}
			lastColor = color_;
			GenerateBlock(j, i, color_, false);
		}
	}
}

/*-------------------------------------------*/
// ブロックの破壊関数
/*-------------------------------------------*/
bool BlockManager::BreakBlock(int _row, int _column)
{
	bool ret = false;
	Block* result = nullptr;
	if (SearchBlock(_row, _column, &result))
	{
		if (result->GetColor() == BlockManager::Color::Obstacle)return false;

		result->BreakMe();
		ret = true;
		SetStatus(State::Break);
	}
	return ret;
}

void BlockManager::BreakBlock(int _elementNum)
{
	// unknown
}

/*-------------------------------------------*/
// 落ちるブロックをチェックする
/*-------------------------------------------*/
void BlockManager::CheckDownBlock()
{
	for (int c = BOARD_COLUMN_MAX - 2; c > -1; --c) // 一番下は無条件で落ちないため -2
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (sortBlocks[c + 1][r].GetColor() == -1 && !sortBlocks[c][r].GetIsFall() || sortBlocks[c + 1][r].GetIsFall())
			{
				Block* result = nullptr;
				if (SearchBlock(r, c, &result))
				{
					result->SetIsFall(true);
				}
			}
		}
	}
}

/*-------------------------------------------*/
// BlockManager::sortBlocks を設定する
/*-------------------------------------------*/
void BlockManager::SetSortBlocks()
{
	Block dummy;
	for (int i = 0; i < BOARD_COLUMN_MAX; i++)
	{
		for (int j = 0; j < BOARD_ROW_MAX; j++)
		{
			sortBlocks[i][j] = dummy;
			Block* result = nullptr;
			if (SearchBlock(j, i, &result))
			{
				sortBlocks[i][j] = *result;
			}
		}
	}
}




#pragma region Process Function
/*-------------------------------------------*/
// Waitの処理
/*-------------------------------------------*/
void BlockManager::WaitProcess()
{
	// unknown
}

/*-------------------------------------------*/
// Breakの処理
/*-------------------------------------------*/
void BlockManager::BreakProcess()
{
	// ブロックが落下するかを判定する
	CheckDownBlock();

	int count = 0;
	for (auto& it : blocks)
	{
		if (it.GetIsFall())
		{
			count++;
		}
	}
	if (count == 0)
	{
		SetStatus(State::Chain);
	}
}

/*-------------------------------------------*/
// Chainの処理
/*-------------------------------------------*/
void BlockManager::ChainProcess(int _pn)
{
	// ブロックが落下するかを判定する
	CheckDownBlock();

	if (CHAIN_RAG <= waitTime[0]++)
	{
		RagisterChainBlock(_pn);
		if (eraseBlockCount != 0)
		{
			regularGameUI[_pn].CalcScore(eraseBlockCount, chainCount);
			eraseBlockCount = 0;
		}

		waitTime[0] = 0;

		if (isChainContinued)
		{
			chainCount++;
			regularGameUI[_pn].SetNowChainNum(chainCount);
			if (sceneSelect.JudgeGameMode(SelectGameMode::Multi) || sceneSelect.JudgeGameMode(SelectGameMode::CPU))
			{
				AttackForOppenent(_pn);
			}
		}
		else
		{
			//chainCount = 0;

			if (lastStatus == State::CheckUpCombo)
			{
				chainCount = 0;
				regularGameUI[_pn].SetIsTimerStop(false);
				SetStatus(State::Wait);
			}
			else
			{
				SetStatus(State::PushUp);
			}
		}
	}
}

/*-------------------------------------------*/
// PopRowLineの処理
/*-------------------------------------------*/
void BlockManager::PopRowLineProcess()
{

	// 下一段のブロック情報をコピーする配列
	std::array<Block, ROW_MAX> copyBlocks;

	// コピーしていく
	bool generateCopyBlock[ROW_MAX] = { false };
	bool pushBack = false;

	for (int i = 0; i < ROW_MAX; i++)
	{
		pushBack = false;
		for (int j = 0; j < BLOCK_NUM_MAX; j++)
		{
			if (i == blocks[j].GetRow() && 8 == blocks[j].GetColumn())
			{
				copyBlocks[i] = blocks[j];
				pushBack = true;
				break;
			}
		}
		if (!pushBack)
		{
			Block b;
			b.SetColor(rand() % colorMax);//TODO : Set colorMax
			b.SetColumn(8);
			b.SetRow(i);
			copyBlocks[i] = b;
			generateCopyBlock[i] = true;
		}
	}

	// 左右のブロックと同じ色ならもう一度ランダムを回す
	for (int i = 0; i < ROW_MAX; i++)
	{
		if (generateCopyBlock[i])
		{
			if (0 < i && i < 5)
			{
				while (copyBlocks[i - 1].GetColor() == copyBlocks[i].GetColor() || copyBlocks[i + 1].GetColor() == copyBlocks[i].GetColor())
				{
					copyBlocks[i].SetColor(rand() % colorMax);
				}
			}
			else if (i == 0)
			{
				while (copyBlocks[i + 1].GetColor() == copyBlocks[i].GetColor())
				{
					copyBlocks[i].SetColor(rand() % colorMax);
				}
			}
			else if (i == 5)
			{
				while (copyBlocks[i - 1].GetColor() == copyBlocks[i].GetColor())
				{
					copyBlocks[i].SetColor(rand() % colorMax);
				}
			}
		}
	}

	// 構造体に確率などをいれる
	struct NextBlock
	{
		int color;
		int par;
	};

	int lastColor = -1;
	for (int i = 0; i < ROW_MAX; i++)
	{
		NextBlock nb[3] = {};
		int popColor = -1;

		if (0 == i) // 左端
		{
			nb[2].color = copyBlocks[i + 1].GetColor();
			nb[2].par = END_MAX_PAR;
			nb[1].color = rand() % colorMax;
			while (nb[1].color == copyBlocks[i].GetColor() || nb[1].color == nb[2].color)
			{
				nb[1].color = rand() % colorMax;
			}
			nb[1].par = END_MIN_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nb[2].color && j != nb[1].color && j != copyBlocks[i].GetColor())
				{
					nb[0].color = j;
				}
			}
			nb[0].par = END_MIN_PAR;
		}
		else if (1 <= i && i <= 4) // 真ん中4つ
		{
			nb[0].color = copyBlocks[i - 1].GetColor();
			nb[0].par = MAX_PAR;
			nb[2].color = copyBlocks[i + 1].GetColor();
			nb[2].par = MAX_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nb[0].color && j != nb[2].color && j != copyBlocks[i].GetColor())
				{
					nb[1].color = j;
				}
			}
			nb[1].par = MIN_PAR;
		}
		else // 右端
		{
			nb[0].color = copyBlocks[i - 1].GetColor();
			nb[0].par = END_MAX_PAR;
			nb[1].color = rand() % colorMax;
			while (nb[1].color == copyBlocks[i].GetColor() || nb[1].color == nb[0].color)
			{
				nb[1].color = rand() % colorMax;
			}
			nb[1].par = END_MIN_PAR;
			for (int j = 0; j < colorMax; j++)
			{
				if (j != nb[0].color && j != nb[1].color && j != copyBlocks[i].GetColor())
				{
					nb[2].color = j;
				}
			}
			nb[2].par = END_MIN_PAR;
		}

		// 最終的なcolorを設定する

		int rnd_ = (rand() % nb[0].par) + (rand() % nb[1].par) + (rand() % nb[2].par);
		int num = -1;
		if (rnd_ <= nb[0].par)
		{
			popColor = nb[0].color;
			num = 0;
		}
		else if (rnd_ <= nb[0].par + nb[1].par)
		{
			popColor = nb[1].color;
			num = 1;
		}
		else if (rnd_ <= nb[0].par + nb[1].par + nb[2].par)
		{
			popColor = nb[2].color;
			num = 2;
		}

		while (lastColor != -1 && lastColor == popColor)
		{
			int remainTwoColor[2] = { -1 };
			int num_ = 0;
			for (int j = 0; j < 3; j++)
			{
				if (j != num)
				{
					remainTwoColor[num_] = j;
					num_++;
				}
			}

			rnd_ = (rand() % nb[remainTwoColor[0]].par) + (rand() % nb[remainTwoColor[1]].par);
			num = -1;
			if (rnd_ <= nb[remainTwoColor[0]].par)
			{
				popColor = nb[remainTwoColor[0]].color;
				num = remainTwoColor[0];
			}
			else if (rnd_ <= nb[remainTwoColor[0]].par + nb[remainTwoColor[1]].par)
			{
				popColor = nb[remainTwoColor[1]].color;
				num = remainTwoColor[1];
			}
		}

		lastColor = popColor;
		GenerateBlock(i, 9, popColor, false);
	}


	SetStatus(State::CheckUpCombo);
}

/*-------------------------------------------*/
// PushUpの処理
/*-------------------------------------------*/
void BlockManager::PushUpProcess(int _pn)
{
	if (!isPushing)
	{
		PreparationPushUp(_pn);
	}
	else
	{
		MovePushUpBoard(_pn);
	}
}

/*-------------------------------------------*/
// CheckUpComboの処理
/*-------------------------------------------*/
void BlockManager::CheckUpComboProcess(int _pn)
{
	// 現状のブロックの配置を記録
	SetSortBlocks();

	// 連鎖している所があるか走査
	if (RagisterUpComboBlock())
	{
		SetStatus(State::Chain);
		regularGameUI[_pn].SetIsTimerStop(true);
	}
	else
	{
		chainCount = 0;
		regularGameUI[_pn].SetIsTimerStop(false);
		SetStatus(State::Wait);
	}
}

/*-------------------------------------------*/
// FallObstacleの処理
/*-------------------------------------------*/
void BlockManager::FallObstacleProcess(int _pn)
{
	CheckDownObstacle();

	for (auto& it : blocks)
	{
		if (it.GetColor() == Color::Obstacle &&
			it.GetIsFall())
		{
			regularGameUI[_pn].SetIsTimerStop(true);
			return;
		}
	}

	regularGameUI[_pn].SetIsTimerStop(false);
	SetStatus(State::Wait);

	//TODO : 準備がされているなら、smokeLを発生させる
	if (regularEffects[_pn].GetIsReadySmokeL())
	{
		regularEffects[_pn].GenerateSmokeL(_pn);
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::LANDING_LARGE_OBSTACLE].get());
	}
}

#pragma endregion 




/*-------------------------------------------*/
// チェイン関係の処理関数
/*-------------------------------------------*/
void BlockManager::RagisterChainBlock(int _pn)
{
	// Local variable
	bool currEraseNum[BLOCK_NUM_MAX] = { false };
	bool setIsChainFlag = false;

	// ラムダ式の数々
	auto CheckUp = [&](int row, int column)
	{
		if (sortBlocks[column - 1][row].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckDown = [&](int row, int column)
	{
		if (sortBlocks[column + 1][row].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckLeft = [&](int row, int column)
	{
		if (sortBlocks[column][row - 1].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckRight = [&](int row, int column)
	{
		if (sortBlocks[column][row + 1].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};

	auto RasisterNum = [&](int row, int column)
	{
		int elementNum = 0;
		SearchBlockNum(row, column, elementNum);
		currEraseNum[elementNum] = true;
	};

	auto SetChainFlagAndPos = [&](int r, int c)
	{
		if (!setIsChainFlag)
		{
			setIsChainFlag = true;

			Block* ans = nullptr;
			SearchBlock(r, c, &ans);
			//provisionalGameUI.SetShowChainNumPos(ans->GetPos());
			//provisionalGameUI.SetIsShowChainNum(true);
			regularGameUI[_pn].SetShowChainNumInit(ans->GetPos());
			regularEffects[_pn].GenerateMiniChar(r, c, _pn);
		}
	};

	auto EraseObstacleBlocks = [&](int row, int column, int pn, bool searchUp, bool searchRight, bool searchDown, bool searchLeft)
	{
		if (searchUp)
		{
			if (sortBlocks[column - 1][row].GetColor() == BlockManager::Color::Obstacle)
			{
				RasisterNum(row, column - 1);
			}
		}

		if (searchDown)
		{
			if (sortBlocks[column + 1][row].GetColor() == BlockManager::Color::Obstacle)
			{
				RasisterNum(row, column + 1);
			}
		}

		if (searchRight)
		{
			if (sortBlocks[column][row + 1].GetColor() == BlockManager::Color::Obstacle)
			{
				RasisterNum(row + 1, column);
			}
		}

		if (searchLeft)
		{
			if (sortBlocks[column][row - 1].GetColor() == BlockManager::Color::Obstacle)
			{
				RasisterNum(row - 1, column);
			}
		}
	};

	// ここから走査開始

	for (int c = 0; c < BOARD_COLUMN_MAX; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (-1 == sortBlocks[c][r].GetColor()) continue;

			if (r == 0) // 左列
			{
				if (c == 0) // 上端
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, true, true, false);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, true, true, false);
					}
				}
				else if (c == 8) //下端
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, false);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, false);
					}
				}
				else
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, false);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, false);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, false);
					}
				}
			}
			else if (r == 5) // 右列
			{
				if (c == 0) // 上端
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, false, true, true);

					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, false, true, true);
					}
				}
				else if (c == 8) // 下端
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, false, false, true);

					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, false, false, true);
					}
				}
				else
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, false, true, true);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, false, true, true);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, false, true, true);
					}
				}
			}
			else
			{
				if (c == 0)
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, true, true, true);

					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, true, true, true);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, false, true, true, true);
					}
				}
				else if (c == 8)
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, true);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, true);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, false, true);
					}
				}
				else
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, true, true);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, true, true);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, true, true);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
						EraseObstacleBlocks(r, c, _pn, true, true, true, true);
					}
				}
			}
		}
	}


	// 走査した結果を反映

	isChainContinued = false; // コンボが一つもない場合falseのまま関数を抜ける
	for (int i = 0; i < BLOCK_NUM_MAX; i++)
	{
		if (currEraseNum[i])
		{
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::DESTRUCTION1].get());

			regularEffects[_pn].GenerateParticle(blocks[i].GetRow(), blocks[i].GetColumn(), blocks[i].GetColor());
			blocks[i].BreakMe();
			eraseBlockCount++;

			// チェインを継続する
			!isChainContinued ? isChainContinued = true : isChainContinued = isChainContinued;
		}
	}

	// Play SE
	if (isChainContinued)
	{
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::DESTRUCTION1].get());
	}

	// 一度にたくさん消したら、その分多く降らせる
	if (IF_MULTI_NOW)
	{
		AddObstacleByBreakNum(_pn);
	}

}

bool BlockManager::RagisterUpComboBlock()
{
	// Local variable
	bool currEraseNum[BLOCK_NUM_MAX] = { false };

	// ラムダ式の数々
	auto CheckUp = [&](int row, int column)
	{
		if (sortBlocks[column - 1][row].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckDown = [&](int row, int column)
	{
		if (sortBlocks[column + 1][row].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckLeft = [&](int row, int column)
	{
		if (sortBlocks[column][row - 1].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};
	auto CheckRight = [&](int row, int column)
	{
		if (sortBlocks[column][row + 1].GetColor() == sortBlocks[column][row].GetColor() && sortBlocks[column][row].GetColor() != Color::Obstacle)
			return true;

		return false;
	};

	auto RasisterNum = [&](int row, int column)
	{
		int elementNum = 0;
		SearchBlockNum(row, column, elementNum);
		currEraseNum[elementNum] = true;
	};


	// ここから走査開始

	for (int c = 0; c < BOARD_COLUMN_MAX; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (-1 == sortBlocks[c][r].GetColor()) continue;

			if (r == 0) // 左列
			{
				if (c == 0) // 上端
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else if (c == 8) //下端
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
				}
			}
			else if (r == 5) // 右列
			{
				if (c == 0) // 上端
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else if (c == 8) // 下端
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
				}
			}
			else
			{
				if (c == 0)
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else if (c == 8)
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
				}
				else
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
					}
				}
			}
		}
	}


	// 走査した結果を反映

	for (int i = 0; i < BLOCK_NUM_MAX; i++)
	{
		if (currEraseNum[i])
		{
			return true;
		}
	}
	return false;
}


/*------------------------------------------*/
// ブロックを上げる前の準備
/*------------------------------------------*/
void BlockManager::PreparationPushUp(int _pn)
{
	for (auto& it : blocks)
	{
		if (0 == it.GetColumn())
		{
			// TODO :  Go to GameOver
#if 0 
			if (PRODUCTION->CheckFlag(GO_TITLE)) continue;
			PRODUCTION->SetOn(GO_TITLE);
			PRODUCTION->Start();
#else
			if (IF_SINGLE_NOW) {
				sceneSingleGame.isGameover = true;
			}
			else if (IF_MULTI_NOW) {
				sceneMultiGame.isGameover[_pn] = true;
			}
#endif
		}
		it.SetColumn(it.GetColumn() - 1);
	}
	isPushing = true;
}


/*------------------------------------------*/
// ブロックを一段上げる
/*------------------------------------------*/
void BlockManager::MovePushUpBoard(int _pn)
{
	// 6フレームかけて上にあげていく
	for (auto& it : blocks)
	{
		it.SetPosY(it.GetPos().y - Block::SIZE_X / 6.0f);
	}

	// プレイヤーのカーソルを持ち上げる
	auto tmpPos = regularPlayer[_pn].GetPos();
	tmpPos.y -= Block::SIZE_X / 6.0f;
	regularPlayer[_pn].SetPos(tmpPos);
	regularPlayer[_pn].SetCanMove(false);


	pushingCount++;

	// 上げ終わる
	if (6 <= pushingCount)
	{
		pushingCount = 0;
		SetStatus(State::PopRowLine);

		// プレイヤー関係
		regularPlayer[_pn].column--;
		regularPlayer[_pn].SetCanMove(true);

		isPushing = false;
		if (!regularGameUI[_pn].GetIsGaugeMax())
		{
			regularGameUI[_pn].SetIsTimerStop(false);
		}
	}
}


/*------------------------------------------*/
// 相手への攻撃関数(For MultiPlay)
/*------------------------------------------*/
void BlockManager::AttackForOppenent(int _pn)
{
	auto AddObstacleNum = [&](int _pn)
	{
		regularBlockManager[_pn].obstacleNum++;
		regularBlockManager[_pn].obstacleKeepTime = 0;
		regularBlockManager[_pn].isObstacleKeeping = true;
	};

	auto SubObstacleNum = [&](int _pn)
	{
		regularBlockManager[_pn].obstacleNum--;
		if (regularBlockManager[_pn].obstacleNum <= 0)
		{
			regularBlockManager[_pn].obstacleNum = 0;
			regularBlockManager[_pn].obstacleKeepTime = 0;
			regularBlockManager[_pn].isObstacleKeeping = false;
		}
	};

	switch (_pn)
	{
	case 0:
		if (chainCount >= 2)
		{
			if (obstacleNum > 0)
			{
				SubObstacleNum(0);
			}
			else
			{
				AddObstacleNum(1);
			}
		}
		break;

	case 1:
		if (chainCount >= 2)
		{
			if (obstacleNum > 0)
			{
				SubObstacleNum(1);
			}
			else
			{
				AddObstacleNum(0);
			}
		}
		break;
	}
}


/*------------------------------------------*/
// カウントいっぱいになったら、お邪魔を降らせる
/*------------------------------------------*/
void BlockManager::SetObstacleWhenCountReaches(int _pn)
{
	if (!isObstacleKeeping || status == State::PushUp) return;

	if (++obstacleKeepTime >= OBSTACLE_KEEPING_COUNT_MAX)
	{
		if (status != State::Wait)return;

		// お邪魔ブロックを降らせるステートに以降する
		status = State::FallObstacle;

		fallObstacleNum = SetNumOfObstacle(obstacleNum);
		SetFallObstacle(fallObstacleNum, _pn);

		regularEffects[_pn].SetStackObstacleNumber(0);
		fallObstacleNum = 0;
		obstacleKeepTime = 0;
		obstacleNum = 0;
		isObstacleKeeping = false;
	}
	else if (obstacleNum == 0)
	{
		obstacleKeepTime = 0;
		isObstacleKeeping = false;
	}
}


/*------------------------------------------*/
// obstacleCountによって降らせる数を変える
// return : Fall of obstacle count.
/*------------------------------------------*/
int BlockManager::SetNumOfObstacle(int _num)
{
	switch (_num)
	{
	case 0:
		return 0;	//break;
	case 1:
		return 0;	//break;
	case 2:
		return 1;	//break;
	case 3:
		return 2;	//break;
	case 4:
		return 4;	//break;
	case 5:
		return 6;	//break;
	case 6:
		return 8;	//break;
	case 7:
		return 11;	//break;
	case 8:
		return 14;	//break;
	case 9:
		return 18;	//break;
	default:
		return 20;	//break;
	}
}


/*------------------------------------------*/
// お邪魔ブロックを降らせる(For MultiPlay)
/*------------------------------------------*/
void BlockManager::SetFallObstacle(int _fallObstacleNum, int _pn)
{
	int checkBoard[10][6];
	//int fallObstacleBlockRow[20];
	std::array<int, 20> fallObstacleBlockRow;
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
		for (size_t i = 0; i < blocks.size(); i++)
		{
			int row = blocks[i].GetRow();
			int clm = blocks[i].GetColumn();
			if (row < 0 || row > 5)continue;
			if (clm < 0 || clm > 9)continue;
			checkBoard[blocks[i].GetColumn()][blocks[i].GetRow()] = enable;	// 要素があるところにはenableを入れる
		}

		// 右下から見て、dummyがあるところの上にお邪魔を生成する
		bool getArea = false;
		for (int i = 8; i > -1; i--)
		{
			for (int j = 5; j > -1; j--)
			{
				if (checkBoard[i][j] == dummy)
				{
					//fallObstacleBlockRow[count++] = j;
					fallObstacleBlockRow[k] = j;
					checkBoard[i][j] = enable;
					getArea = true;
				}
				if (getArea)break;
			}
			if (getArea)break;
		}
	}

	// SmokeLを出す場合は準備をする
	if (_fallObstacleNum >= Effect::BORDER_OF_SMOKEL)
	{
		regularEffects[_pn].SetIsReadySmokeL(true);
	}





	std::array<int, 6> overlapCount{ 0 };

	int generateRowSave[20];	// 生成したrowを保持する
	for (auto& it : generateRowSave)
	{
		it = -1;
	}

	// お邪魔ブロックを生成する
	int generateCount = 0;
	dummy = -1;
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
		GenerateBlock(fallObstacleBlockRow[i], -2 - overlapCount[fallObstacleBlockRow[i]], Color::Obstacle); // 生成時に画面外に行くように、-2

		for (size_t k = blocks.size() - 1; k < -1; k--)
		{
			if (blocks[k].GetColor() == Color::Obstacle)
			{
				blocks[k].SetIsFall(true);
				break;
			}
		}
		generateRowSave[generateCount++] = fallObstacleBlockRow[i];
	}
}


/*---------------------------------------------------*/
// お邪魔ブロックが持続して落ちるかの判定(For MultiPlay)
/*---------------------------------------------------*/
void BlockManager::CheckDownObstacle()
{
	Block sortAllBlock[COLUMN_MAX - 1][ROW_MAX]; // 下段一段分が必要ないので -1
	Block dummy{};
	for (int c = 0; c < COLUMN_MAX - 1; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			sortAllBlock[c][r] = dummy;
			Block* result = nullptr;
			if (SearchBlock(r, c - 9, &result))
			{
				sortAllBlock[c][r] = *result;
			}
		}
	}

	for (int c = 0; c < COLUMN_MAX - 1 - 1; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (sortAllBlock[c + 1][r].GetColor() == -1 && !sortAllBlock[c][r].GetIsFall() || sortAllBlock[c + 1][r].GetIsFall())
			{
				Block* result = nullptr;
				if (SearchBlock(r, c - 9, &result))
				{
					if (result->GetColor() == Color::Obstacle)
						result->SetIsFall(true);
				}
			}
		}
	}
}


/*----------------------------------------------------*/
// 壊したブロックの数に応じてお邪魔が増える(For MultiPlay)
/*----------------------------------------------------*/
void BlockManager::AddObstacleByBreakNum(int _pn)
{
	auto AddObstacle = [&](int _breakNum)
	{
		switch (_breakNum)
		{
		case 3:
			return 2;

		case 4:
			return 2;

		case 5:
			return 3;

		case 6:
			return 4;

		case 7:
			return 5;

		case 8:
			return 6;

		case 9:
			return 7;

		case 10:
			return 8;

		default:
			return 0;

		}
	};

	auto ResetObstacle = [&](BlockManager& bm)
	{
		bm.obstacleNum = 0;
		bm.obstacleKeepTime = 0;
		bm.isObstacleKeeping = false;

	};


	auto& bm0 = regularBlockManager[0];
	auto& bm1 = regularBlockManager[1];
	switch (_pn)
	{
	case 0:
		// 自分のobstacleNumがあったら先に自分のやつを減らす
		if (bm0.obstacleNum > 0)
		{
			bm0.obstacleNum -= AddObstacle(bm0.eraseBlockCount);
			if (bm0.obstacleNum <= 0)
			{
				ResetObstacle(bm0);
			}
		}
		else
		{
			bm1.obstacleNum += AddObstacle(bm0.eraseBlockCount);
			bm1.isObstacleKeeping = true;
		}
		break;

	case 1:
		// 自分のobstacleNumがあったら先に自分のやつを減らす
		if (bm1.obstacleNum > 0)
		{
			bm1.obstacleNum -= AddObstacle(bm1.eraseBlockCount);
			if (bm1.obstacleNum <= 0)
			{
				ResetObstacle(bm1);
			}
		}
		else
		{
			bm0.obstacleNum += AddObstacle(bm1.eraseBlockCount);
			bm0.isObstacleKeeping = true;
		}
		break;
	}
}




/*------------------------------------------*/
// 引数と同じ場所にあるBlockを返す
// return : (search sucess?) true : false
/*------------------------------------------*/
bool BlockManager::SearchBlock(int _row, int _column, Block** _ans)
{
	for (int i = 0; i < BLOCK_NUM_MAX; ++i)
	{
		if (blocks[i].GetRow() == _row && blocks[i].GetColumn() == _column)
		{
			*_ans = &blocks[i];
			return true;
		}
	}
	return false;
}

/*------------------------------------------*/
// 引数と同じ場所にあるBlockを返す
// return : (search sucess?) true : false
/*------------------------------------------*/
bool BlockManager::SearchAliveBlock(int _row, int _column, Block** _ans)
{
	for (int i = 0; i < BLOCK_NUM_MAX; ++i)
	{
		if (blocks[i].GetColumn() == 9 || blocks[i].GetColor() == Color::Obstacle)
		{
			continue;
		}
		if (blocks[i].GetRow() == _row && blocks[i].GetColumn() == _column)
		{
			*_ans = &blocks[i];
			return true;
		}
	}
	return false;
}

/*------------------------------------------*/
// 引数と同じ場所にあるBlockの要素番号を返す
// return : (search sucess?) true : false
/*------------------------------------------*/
bool BlockManager::SearchBlockNum(int _row, int _column, int& _ans)
{
	for (int i = 0; i < BLOCK_NUM_MAX; ++i)
	{
		if (blocks[i].GetRow() == _row && blocks[i].GetColumn() == _column)
		{
			_ans = i;
			return true;
		}
	}
	return false;
}

void BlockManager::UseImGui()
{
	ImGui::Begin("obstacle");

	ImGui::Text("player1");
	ImGui::SliderInt("obstacleNum", &regularBlockManager[0].obstacleNum, 0, 100);
	ImGui::SliderInt("keeptime", &regularBlockManager[0].obstacleKeepTime, 0, 180);
	ImGui::Checkbox("isObstacleKeeping", &regularBlockManager[0].isObstacleKeeping);
	ImGui::End();

	//ImGui::Text(" ");

	ImGui::Begin("obstacle2");

	ImGui::Text("player2");
	ImGui::SliderInt("obstacleNum", &regularBlockManager[1].obstacleNum, 0, 100);
	ImGui::SliderInt("keeptime", &regularBlockManager[1].obstacleKeepTime, 0, 180);
	ImGui::Checkbox("isObstacleKeeping", &regularBlockManager[1].isObstacleKeeping);

	ImGui::End();
}


