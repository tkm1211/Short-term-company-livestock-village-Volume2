#include "BlockManager.h"
#include "Resource.h"

#include <input_device.h> //TODO:後で消す
#include <array>
#include "SceneManager.h"
#include "Production.h"
#include "UI.h"
#include "Effect.h"
/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

std::array<BlockManager, 2> regularBlockManager;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void BlockManager::Init(int _pn)
{
	sprBlock = RESOURCE->GetSpriteData(Resource::Texture::Block);

	for (auto& it : blocks)
	{
		it.Init();
	}

	GenerateBlock(0, 5, Color::Green);
	GenerateBlock(1, 5, Color::Yellow);
	GenerateBlock(2, 5, Color::LightBlue);
	GenerateBlock(3, 5, Color::Red);
	GenerateBlock(4, 5, Color::Green);
	GenerateBlock(5, 5, Color::Yellow);

	GenerateBlock(0, 6, Color::Yellow);
	GenerateBlock(1, 6, Color::LightBlue);
	GenerateBlock(2, 6, Color::Red);
	GenerateBlock(3, 6, Color::Green);
	GenerateBlock(4, 6, Color::Yellow);
	GenerateBlock(5, 6, Color::LightBlue);

	GenerateBlock(0, 7, Color::LightBlue);
	GenerateBlock(1, 7, Color::Red);
	GenerateBlock(2, 7, Color::Green);
	GenerateBlock(3, 7, Color::Red);
	GenerateBlock(4, 7, Color::LightBlue);
	GenerateBlock(5, 7, Color::Yellow);

	GenerateBlock(0, 8, Color::Red);
	GenerateBlock(1, 8, Color::Green);
	GenerateBlock(2, 8, Color::Yellow);
	GenerateBlock(3, 8, Color::LightBlue);
	GenerateBlock(4, 8, Color::Green);
	GenerateBlock(5, 8, Color::Red);

	PopRowLineProcess();


	// Member variable initialize.
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
	if (sceneSelect.JudgeGameMode(SceneSelect::SelectGameMode::Single))
	{
		ProcessOfSingleGame();
	}
	else if (sceneSelect.JudgeGameMode(SceneSelect::SelectGameMode::Multi))
	{
		ProcessOfMultiGame(_pn);
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

		sprBlock->Draw(it.GetPos().x + ADJUST + SINGLE_CORRECTION_X, it.GetPos().y + ADJUST + SINGLE_CORRECTION_Y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
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

			sprBlock->Draw(it.GetPos().x + ADJUST + GameUI::MULTIPLAY_ONE_ORIJIN_X, it.GetPos().y + ADJUST + GameUI::MULTI_CORRECTION_Y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
		}
		break;

	case 1:
		for (auto& it : blocks)
		{
			if (9 <= it.GetColumn() || !it.GetIsExist())
			{
				continue;
			}

			sprBlock->Draw(it.GetPos().x + ADJUST + GameUI::MULTIPLAY_TWO_ORIJIN_X, it.GetPos().y + ADJUST + GameUI::MULTI_CORRECTION_Y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
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
		//SetObstacleWhenCountReaches(_pn);
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

	for (auto& it : blocks)
	{
		it.Update(_pn);
	}

}

/*-------------------------------------------*/
// ブロックの生成関数
/*-------------------------------------------*/
void BlockManager::GenerateBlock(int _row, int _column, int _color)
{
	for (auto& it : blocks)
	{
		if (it.GetIsExist()) continue;

		// If, GetIsExist is False.
		it.GenerateMe(_row, _column, _color);
		break;
	}
}

/*-------------------------------------------*/
// スタート時のブロック設定
/*-------------------------------------------*/
void BlockManager::SetStartBlock()
{
	// unknown
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
		for (int r = 0;r<BOARD_ROW_MAX;r++)
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
			if (sceneSelect.JudgeGameMode(SceneSelect::Multi))
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
					copyBlocks[i].SetColor( rand() % colorMax);
				}
			}
			else if (i == 0)
			{
				while (copyBlocks[i + 1].GetColor() == copyBlocks[i].GetColor())
				{
					copyBlocks[i].SetColor(rand() % colorMax);
				}
			}
			else if(i == 5)
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
		GenerateBlock(i, 9, popColor);
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
		PreparationPushUp();
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
}

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
			regularEffects[_pn].GenerateParticle(blocks[i].GetRow(), blocks[i].GetColumn(), blocks[i].GetColor());
			blocks[i].BreakMe();
			eraseBlockCount++;

			// チェインを継続する
			!isChainContinued ? isChainContinued = true : isChainContinued = isChainContinued;
		}
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
void BlockManager::PreparationPushUp()
{
	for (auto& it : blocks)
	{
		if (0 == it.GetColumn())
		{
			// TODO :  Go to GameOver
			if (PRODUCTION->CheckFlag(GO_TITLE)) continue;
			PRODUCTION->SetOn(GO_TITLE);
			PRODUCTION->Start();
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
		it.SetPosY(it.GetPos().y - 114.0f / 6.0f);
	}

	pushingCount++;

	// 上げ終わる
	if (6 <= pushingCount)
	{
		pushingCount = 0;
		SetStatus(State::PopRowLine);

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
	switch (_pn)
	{
	case 0:
		if ( chainCount >= 2 )
		{
			regularBlockManager[1].obstacleNum++;
			regularBlockManager[1].obstacleKeepTime = 0;
			regularBlockManager[1].isObstacleKeeping = true;
		}
		break;

	case 1:
		if ( chainCount >= 2 )
		{
			regularBlockManager[0].obstacleNum++;
			regularBlockManager[0].obstacleKeepTime = 0;
			regularBlockManager[0].isObstacleKeeping = true;
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
		
		SetFallObstacle(SetNumOfObstacle(obstacleNum), _pn);
		obstacleKeepTime = 0;
		obstacleNum = 0;
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

		for (size_t k = blocks.size()-1; k < -1; k--)
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


/*------------------------------------------*/
// (For MultiPlay)
/*------------------------------------------*/
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


