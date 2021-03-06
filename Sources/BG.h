#pragma once

#include <memory>

#include "sprite.h"
#include "Singleton.h"

class BG : public Singleton<BG>
{
private:
	std::shared_ptr<SpriteBatch> sprBG;
	std::shared_ptr<SpriteBatch> sprPlayerBack;
	std::shared_ptr<SpriteBatch> sprGrid;
	std::shared_ptr<SpriteBatch> sprRedFrame;
	std::shared_ptr<SpriteBatch> sprBlueFrame;

public:
	BG() { Init(); }
	~BG() { Uninit(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawOfSingle();
	void DrawOfMulti();
};

#define BG_INSTANCE BG::getInstance()
