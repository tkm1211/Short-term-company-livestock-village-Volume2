#include "Block.h"
#include "SceneManager.h"
#include "Effect.h"
#include "Sound.h"

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
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::LANDING_SMALL_OBSTACLE].get());
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
		isFall = false;//TODO:とりま
		++column;
		moveCount = 0;
	}

}

void Block::GenerateMe(int _row, int _column, int _color, bool _isFall)
{
	// 念の為初期化
	Init();

	row = _row;
	column = _column;
	color = _color;
	pos = DirectX::XMFLOAT2((float)(row * SIZE_X), (float)(column * SIZE_Y));

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
