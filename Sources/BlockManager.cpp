#include "BlockManager.h"
#include "Resource.h"

#include <input_device.h> //TODO:��ŏ���
#include "Production.h"
#include "UI.h"
/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void BlockManager::Init()
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

	blocks;

	// Member variable initialize.
	status = State::Wait;

	for (auto& it : waitTime) it = 0;

	chainCount = 0;
	pushingCount = 0;
	eraseBlockCount = 0;

	isChainContinued = false;
	isPushing = false;
	isPushUpByGauge = false;
	isChainAfterPushUp = false;
}

void BlockManager::Uninit()
{
	for (auto& it : blocks)
	{
		it.Uninit();
	}

}

void BlockManager::Update()
{

	// ��l�p�̍X�V�֐�
	ProcessOfSingleGame();

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


//-----------------------------------------------------------------------

//	�e��֐�

//-----------------------------------------------------------------------
/*-------------------------------------------*/
// �V���O���Q�[���̍X�V�֐�
/*-------------------------------------------*/
void BlockManager::ProcessOfSingleGame()
{
	// �u���b�N��񎟌��z��փ\�[�g����
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
		ChainProcess();
		break;
	case BlockManager::PushUp:
		PushUpProcess();
		break;
	case BlockManager::PopRowLine:
		PopRowLineProcess();
		break;
	case BlockManager::CheckUpCombo:
		CheckUpComboProcess();
		break;
	default:
		break;
	}

	if (isPushUpByGauge && !isPushing)
	{
		SetStatus(State::PushUp);
		//status = State::PushUp;
		isPushUpByGauge = false;
	}

	for (auto& it : blocks)
	{
		it.Update();
	}
}

/*-------------------------------------------*/
// �u���b�N�̐����֐�
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
// �X�^�[�g���̃u���b�N�ݒ�
/*-------------------------------------------*/
void BlockManager::SetStartBlock()
{
	// unknown
}

/*-------------------------------------------*/
// �u���b�N�̔j��֐�
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
// ������u���b�N���`�F�b�N����
/*-------------------------------------------*/
void BlockManager::CheckDownBlock()
{
	for (int c = BOARD_COLUMN_MAX - 2; c > -1; --c) // ��ԉ��͖������ŗ����Ȃ����� -2
	{
		for (int r = 0;r<BOARD_ROW_MAX;r++)
		{
			if (sortBlocks[c + 1][r].GetColor() == -1 && !sortBlocks[c][r].GetIsFall() || sortBlocks[c+1][r].GetIsFall())
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
// BlockManager::sortBlocks ��ݒ肷��
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
// Wait�̏���
/*-------------------------------------------*/
void BlockManager::WaitProcess()
{
	// unknown
}

/*-------------------------------------------*/
// Break�̏���
/*-------------------------------------------*/
void BlockManager::BreakProcess()
{
	// �u���b�N���������邩�𔻒肷��
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
// Chain�̏���
/*-------------------------------------------*/
void BlockManager::ChainProcess()
{
	// �u���b�N���������邩�𔻒肷��
	CheckDownBlock();

	if (CHAIN_RAG <= waitTime[0]++)
	{
		RagisterChainBlock();
		if (eraseBlockCount != 0)
		{
			provisionalGameUI.CalcScore(eraseBlockCount, chainCount);
			eraseBlockCount = 0;
		}

		waitTime[0] = 0;

		if (isChainContinued)
		{
			chainCount++;
			provisionalGameUI.SetNowChainNum(chainCount);
		}
		else
		{
			//chainCount = 0;

			if (lastStatus == State::CheckUpCombo)
			{
				chainCount = 0;
				provisionalGameUI.SetIsTimerStop(false);
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
// PopRowLine�̏���
/*-------------------------------------------*/
void BlockManager::PopRowLineProcess()
{

	// ����i�̃u���b�N�����R�s�[����z��
	std::array<Block, ROW_MAX> copyBlocks;

	// �R�s�[���Ă���
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

	// ���E�̃u���b�N�Ɠ����F�Ȃ������x�����_������
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

	// �\���̂Ɋm���Ȃǂ������
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

		if (0 == i) // ���[
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
		else if (1 <= i && i <= 4) // �^��4��
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
		else // �E�[
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

		// �ŏI�I��color��ݒ肷��

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
// PushUp�̏���
/*-------------------------------------------*/
void BlockManager::PushUpProcess()
{
	if (!isPushing)
	{
		PreparationPushUp();
	}
	else
	{
		MovePushUpBoard();
	}
}

/*-------------------------------------------*/
// CheckUpCombo�̏���
/*-------------------------------------------*/
void BlockManager::CheckUpComboProcess()
{
	// ����̃u���b�N�̔z�u���L�^
	SetSortBlocks();

	// �A�����Ă��鏊�����邩����
	if (RagisterUpComboBlock())
	{
		SetStatus(State::Chain);
		provisionalGameUI.SetIsTimerStop(true);
	}
	else
	{
		chainCount = 0;

		SetStatus(State::Wait);
	}
}



/*-------------------------------------------*/
// �`�F�C���֌W�̏����֐�
/*-------------------------------------------*/
void BlockManager::RagisterChainBlock()
{
	// Local variable
	bool currEraseNum[BLOCK_NUM_MAX] = { false };
	bool setIsChainFlag = false;

	// �����_���̐��X
	auto CheckUp = [&](int row, int column)
	{
		if (sortBlocks[column - 1][row].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckDown = [&](int row, int column)
	{
		if (sortBlocks[column + 1][row].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckLeft = [&](int row, int column)
	{
		if (sortBlocks[column][row - 1].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckRight = [&](int row, int column)
	{
		if (sortBlocks[column][row + 1].GetColor() == sortBlocks[column][row].GetColor())
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
			provisionalGameUI.SetShowChainNumInit(ans->GetPos());
		}
	};

	// �������瑖���J�n

	for (int c = 0; c < BOARD_COLUMN_MAX; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (-1 == sortBlocks[c][r].GetColor()) continue;

			if (r == 0) // ����
			{
				if (c == 0) // ��[
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else if (c == 8) //���[
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
			}
			else if (r == 5) // �E��
			{
				if (c == 0) // ��[
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else if (c == 8) // ���[
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else
				{
					if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
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
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else if (c == 8)
				{
					if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
				else
				{
					if (CheckUp(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckRight(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckDown(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
					else if (CheckLeft(r, c))
					{
						RasisterNum(r, c);
						SetChainFlagAndPos(r, c);
					}
				}
			}
		}
	}


	// �����������ʂ𔽉f

	isChainContinued = false; // �R���{������Ȃ��ꍇfalse�̂܂܊֐��𔲂���
	for (int i = 0; i < BLOCK_NUM_MAX; i++)
	{
		if (currEraseNum[i])
		{
			blocks[i].BreakMe();
			eraseBlockCount++;

			// �`�F�C�����p������
			!isChainContinued ? isChainContinued = true : isChainContinued = isChainContinued;
		}
	}
}

bool BlockManager::RagisterUpComboBlock()
{
	// Local variable
	bool currEraseNum[BLOCK_NUM_MAX] = { false };

	// �����_���̐��X
	auto CheckUp = [&](int row, int column)
	{
		if (sortBlocks[column - 1][row].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckDown = [&](int row, int column)
	{
		if (sortBlocks[column + 1][row].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckLeft = [&](int row, int column)
	{
		if (sortBlocks[column][row - 1].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};
	auto CheckRight = [&](int row, int column)
	{
		if (sortBlocks[column][row + 1].GetColor() == sortBlocks[column][row].GetColor())
			return true;

		return false;
	};

	auto RasisterNum = [&](int row, int column)
	{
		int elementNum = 0;
		SearchBlockNum(row, column, elementNum);
		currEraseNum[elementNum] = true;
	};


	// �������瑖���J�n

	for (int c = 0; c < BOARD_COLUMN_MAX; c++)
	{
		for (int r = 0; r < BOARD_ROW_MAX; r++)
		{
			if (-1 == sortBlocks[c][r].GetColor()) continue;

			if (r == 0) // ����
			{
				if (c == 0) // ��[
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
				else if (c == 8) //���[
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
			else if (r == 5) // �E��
			{
				if (c == 0) // ��[
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
				else if (c == 8) // ���[
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


	// �����������ʂ𔽉f

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
// �u���b�N���グ��O�̏���
/*------------------------------------------*/
void BlockManager::PreparationPushUp()
{
	for (auto& it : blocks)
	{
		if (0 == it.GetColumn())
		{
			// TODO :  Go to GameOver
			PRODUCTION->SetOn(GO_TITLE);
			PRODUCTION->Start();
		}
		it.SetColumn(it.GetColumn() - 1);
	}
	isPushing = true;
}

/*------------------------------------------*/
// �u���b�N����i�グ��
/*------------------------------------------*/
void BlockManager::MovePushUpBoard()
{
	// 6�t���[�������ď�ɂ����Ă���
	for (auto& it : blocks)
	{
		it.SetPosY(it.GetPos().y - 114.0f / 6.0f);
	}

	pushingCount++;

	// �グ�I���
	if (6 <= pushingCount)
	{
		pushingCount = 0;
		SetStatus(State::PopRowLine);

		isPushing = false;
		if (!provisionalGameUI.GetIsGaugeMax())
		{
			provisionalGameUI.SetIsTimerStop(false);
		}
	}
}


/*------------------------------------------*/
// �����Ɠ����ꏊ�ɂ���Block��Ԃ�
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
// �����Ɠ����ꏊ�ɂ���Block�̗v�f�ԍ���Ԃ�
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


