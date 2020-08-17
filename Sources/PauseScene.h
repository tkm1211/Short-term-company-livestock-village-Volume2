#pragma once

#include <memory>
#include <array>
#include <Sprite.h>

class Pause
{
private:
	std::shared_ptr<SpriteBatch> sprPause;
	std::shared_ptr<SpriteBatch> sprBack;

	TexData menuData;
	TexData backData;
	TexData selectData;

	std::array<int, 2> pushCnt;
	bool isPauseNow;

	enum SelectMenu
	{
		BackGame,
		Restart,
		GoTitle,
	};
	int selectMenu;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetIsPauseNow(bool _n) { isPauseNow = _n; }
	bool GetisPauseNow() { return isPauseNow; }
};

extern Pause pause;