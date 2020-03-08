#pragma once

#include <DirectXMath.h>

class Block
{
private:
	static constexpr int FALL_COUNT = 6;
	static constexpr int FALL_SPEED = 114 / FALL_COUNT;//  =19 
	static constexpr int SIZE_X = 114;
	static constexpr int SIZE_Y = 114;

	DirectX::XMFLOAT2 pos;
	int color;		// êF
	int row;		// â°î‘çÜ
	int column;		// ècî‘çÜ
	int moveCount;	// à⁄ìÆâ¬î\Ç»ïbêî

	bool isExist;
	bool isFall;
	bool lastIsFall;
	bool readySmoke;


public:
	Block() { Init(); }

	~Block() { Uninit(); }

	void Init();
	void Uninit();
	void Update(int _pn);
	void Draw();

private:
	void OperateBlock(int _pn);

public:
	void GenerateMe(int _row, int _column, int _color);
	void BreakMe();

public:	// Getter and Setter
	bool	GetIsExist() { return isExist; }
	bool	GetIsFall() { return isFall; }
	int		GetColumn() { return column; }
	int		GetRow() { return row; }
	int		GetSizeX() { return SIZE_X; }
	int		GetSizeY() { return SIZE_Y; }
	int		GetColor() { return color; }
	DirectX::XMFLOAT2 GetPos() { return pos; }

	void SetIsExist(bool _set) { isExist = _set; }
	void SetIsFall(bool _set) { isFall = _set; }
	void SetColumn(int _column) { column = _column; }
	void SetRow(int _row) { row = _row; }
	void SetColor(int _color) { color = _color; }
	void SetPosition(DirectX::XMFLOAT2 _pos) { pos = _pos; }
	void SetPosX(float _x) { pos.x = _x; }
	void SetPosY(float _y) { pos.y = _y; }
};
