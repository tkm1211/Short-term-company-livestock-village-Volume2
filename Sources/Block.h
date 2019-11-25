#pragma once
#include <DirectXMath.h>


class Block
{
public:
	static constexpr int SPEED = 19;
	static constexpr int SIZE_X = 114;
	static constexpr int SIZE_Y = 114;

	DirectX::XMFLOAT2 pos;
	int		color;		// F
	int		column;		// c”Ô†
	int		row;		// ‰¡

	int		elementNum;	// —v‘f”
	int		moveCount;
	int		eraseNum;	// íœ‚·‚é‚Æ‚«‚É‚¢‚é

	bool isEnable;
	bool isFall;
	bool isNextFall;
	bool isErase;		// ˜A½‚µ‚ÄÁ‚¦‚éŒn‚Ì‚â‚Â

public:
	Block()
	{
		pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		color = -1;		// F
		column = -1;		// c”Ô†
		row = -1;		// ‰¡

		elementNum = -1;	// —v‘f”
		moveCount = -1;
		eraseNum = -1;	// íœ‚·‚é‚Æ‚«‚É‚¢‚é

		isEnable = false;
		isFall = false;
		isNextFall = false;
		isErase = false;		// ˜A½‚µ‚ÄÁ‚¦‚éŒn‚Ì‚â‚Â
	}
	~Block()
	{
		pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		color = -1;		// F
		column = -1;		// c”Ô†
		row = -1;		// ‰¡

		elementNum = -1;	// —v‘f”
		moveCount = -1;
		eraseNum = -1;	// íœ‚·‚é‚Æ‚«‚É‚¢‚é

		isEnable = false;
		isFall = false;
		isNextFall = false;
	}
	void Init();
	void Uninit();
	void Update(int);
	void Draw(int);

	void Move();
	void NextMove(int);

	void chackDownBlock(int);
};
