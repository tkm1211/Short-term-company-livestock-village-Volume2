#include "Block.h"
#include "GameMaster.h"

void Block::Init()
{
	isEnable = false;
	isFall = false;
	isNextFall = false;
	moveCount = 0;
	eraseNum = -1;
	elementNum = -1;
}

void Block::Update(int _playerNum)
{
	if (isFall)
	{
		Move();
		if (isNextFall) NextMove(_playerNum);
	}


	if (!isFall && !isNextFall && !GAME_MASTER->blockManager[_playerNum].pushUp)
	{
		pos.y = 114.0f * column;
		chackDownBlock(_playerNum);
	}
//	if (isFall) Move();
//	if (isNextFall) NextMove(_playerNum);
}

void Block::Draw(int _playerNum)
{

}

void Block::Uninit()
{

}


void Block::Move()
{
	pos.y += 114.0f / 6.0f;

	if (++moveCount >= 6)
	{
		++column;
		isNextFall = true;
		moveCount = 0;
	}
}

void Block::NextMove(int _playerNum)
{
	//std::vector<bool> isData;
	if (column < 0) return;

	bool isData[9];
	for (int i = 0; i < 9; i++)
	{
		isData[i] = false;
	}
	int cnt = 0;
	int num = 9 - column;
	for (int i = column; i < 9; i++)
	{
		for (size_t j = 0; j < GAME_MASTER->blockManager[_playerNum].block.size(); j++) // vector‚Ì’†g‚ðŒ©‚é
		{
			int _row = GAME_MASTER->blockManager[_playerNum].block[j].row;
			if (_row == row)
			{
				int _column = GAME_MASTER->blockManager[_playerNum].block[j].column;
				if (_column == i)
				{
					//isData.push_back(true);
					isData[cnt] = true;
					cnt++;
				}
			}
		}
	}

	//if (num == isData.size())
	//{
	//	isFall = false;
	//}

	cnt = 0;
	for (int i = 0; i < 9; i++)
	{
		if (isData[i]) cnt++;
	}

	if (num == cnt)
	{
		isFall = false;
		isNextFall = false;
		moveCount = 0;
	}
}

void Block::chackDownBlock(int _playerNum)
{
	bool isOk = false;
	for (size_t i = 0; i < GAME_MASTER->blockManager[_playerNum].block.size(); i++) // vector‚Ì’†g‚ðŒ©‚é
	{
		int _row = GAME_MASTER->blockManager[_playerNum].block[i].row;
		int _column = GAME_MASTER->blockManager[_playerNum].block[i].column;
		if (_row == row && _column == column)
		{
			for (size_t j = 0; j < GAME_MASTER->blockManager[_playerNum].block.size(); j++) // vector‚Ì’†g‚ðŒ©‚é
			{
				int __row = GAME_MASTER->blockManager[_playerNum].block[i].row;
				int __column = GAME_MASTER->blockManager[_playerNum].block[i].column;
				if (__column == column + 1)
				{
					isOk = true;
					break;
				}
			}
			if (isOk)
			{
				isFall = true;
				break;
			}
		}
	}
}