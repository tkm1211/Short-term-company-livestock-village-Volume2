#include "Block.h"
#include "SceneManager.h"
#include "Effect.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void Block::Init()
{
	pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	color = -1;
	column = -1;
	row = -1;
	moveCount = 0;

	isExist = false;
	isFall = false;
	lastIsFall = false;
	readySmoke = false;
}

void Block::Uninit()
{
	pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	color = -1;
	column = -1;
	row = -1;
	moveCount = 0;

	isExist = false;
	isFall = false;
	lastIsFall = false;
	readySmoke = false;
}

void Block::Update(int _pn)
{
	lastIsFall = isFall;
	if (isFall)
	{
		OperateBlock(_pn);
	}

	if (lastIsFall == false && isFall == false)
	{
		if (readySmoke)
		{
			// Generate smoke
			regularEffects[_pn].GenerateSmoke(row, column);
			readySmoke = false;
		}
	}

}

void Block::Draw()
{

}

void Block::OperateBlock(int _pn)
{
	pos.y += FALL_SPEED;
	if (++moveCount >= FALL_COUNT)
	{
		isFall = false;//TODO:‚Æ‚è‚Ü
		++column;
		moveCount = 0;
	}

}

void Block::GenerateMe(int _row, int _column, int _color, bool _isFall)
{
	// ”O‚Ìˆ×‰Šú‰»
	Init();

	row = _row;
	column = _column;
	color = _color;
	pos = DirectX::XMFLOAT2(row * SIZE_X, column * SIZE_Y);

	if (color == 7)
	{
		readySmoke = true;
	}

	isFall = _isFall;
	isExist = true;
}

void Block::BreakMe()
{
	Uninit();
}
