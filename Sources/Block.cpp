#include "Block.h"

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
	moveCount = -1;

	isExist = false;
	isFall = false;
}

void Block::Uninit()
{

}

void Block::Update()
{

}

void Block::Draw()
{

}

void Block::OperateBlock()
{
	pos.y += FALL_SPEED;
	if (++moveCount >= FALL_COUNT)
	{
		++column;
		moveCount = 0;
	}
}

void Block::GenerateMe(int _row, int _column, int _color)
{
	// �O�̈׏�����
	Init();

	row = _row;
	column = _column;
	color = _color;
	pos = DirectX::XMFLOAT2(row * SIZE_X, column * SIZE_Y);

	isExist = true;
}
